#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

namespace configs{
  int gen_time = 600000;
}

int main(void)
{
  const int rows = 20;
  const int cols = 30;
  size_t generation = 1;
  int current_grid[rows][cols] = {0};
  int updated_grid[rows][cols] = {0};

  current_grid[0][0] = current_grid[0][1] = current_grid[0][2] = 1;

  std::cout << "\033[2J\033[H";
  std::cout<< "Generation: " << generation << "\n"; 
  for (int r = 0; r < rows; r++){
    for (int c = 0; c < cols; c++){
      if (current_grid[r][c] == 0){
        std::cout << "\033[40m-\033[0m";
      } else {
        std::cout << "\033[41m+\033[0m";
      }
    }
    std::cout << "\n";
  }

  while (1){
    usleep(configs::gen_time);

    generation++;
    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        
        int live_count = 0;
        for (int ud = -1; ud < 2; ud++){
          for (int lr = -1; lr < 2; lr++){
            if (ud == 0 && lr == 0)
              continue;
            if ( current_grid[r+ud][c+lr] == 1)
              live_count++;
          }
        }

        if (current_grid[r][c] == 0 && live_count == 3){
          updated_grid[r][c] = 1;
        } else if (current_grid[r][c] == 1 && ( live_count == 2 || live_count == 3)){
          updated_grid[r][c] = 1;
        } else {
          updated_grid[r][c] = 0;
        }
      }
    }

    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        current_grid[r][c] = updated_grid[r][c];
      }
    }

    std::cout << "\033[2J\033[H";
    std::cout<< "Generation: " << generation << "\n"; 
    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        if (current_grid[r][c] == 0){
          std::cout << "\033[40m-\033[0m";
        } else {
          std::cout << "\033[41m+\033[0m";
        }
      }
      std::cout << "\n";
    }
  }
  return 0;
}

