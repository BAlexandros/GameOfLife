#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

// Struct that holds coordinates to make initial grid setting easier
struct point { 
  int x; int y;
  point(int x0, int y0) {
    x = x0; 
    y = y0; 
  };
};

// Collection of configurations for easy access
namespace configs{
  const int gen_time    = 100000/2; // Controls the simulation speed
  const bool fullscreen = false;    // Control whether the whole terminal is used
  const int row_num     = 20;       // If not fullscreen, number of rows
  const int col_num     = 50;       // If not fullscreen, number of cols
  // Collection of cells to be declared as live when starting. If some point is
  // out of the grid it will be ignored
  const std::vector<point> init_live = { point(0,0), point(0,1), point(0,2), \
                                         point(1,1), point(2,1), point(3,2), \
                                         point(3,1), point(4,1), point(5,2), \
                                         point(6,4), point(4,2), point(4,9), \
                                         point(7,4), point(4,7), point(8,1)};
}

int main(void)
{
  int rows;
  int cols;

  // Set grid dimensions according to configuration
  if (configs::fullscreen){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rows = w.ws_row - 2;
    cols = w.ws_col;
  } else {
    rows = configs::row_num;
    cols = configs::col_num;
  }

  // Initialize the grid to all dead, and then set the live cells
  size_t generation = 1;
  int current_grid[rows][cols] = {0};
  int updated_grid[rows][cols] = {0};
  for (auto const& p: configs::init_live){
    if ( p.x < 0 || p.y < 0 || p.x >= rows || p.y >= cols ){
      continue;
    }
    current_grid[p.x][p.y] = 1;
  }

  // Print out the initial grid
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

  // Simulation loop
  while (1){
    usleep(configs::gen_time);

    // Scan the current grid to determine the number of live neighbours for each cell
    generation++;
    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        
        int live_count = 0;
        for (int ud = -1; ud < 2; ud++){
          for (int lr = -1; lr < 2; lr++){
            // Do not count the cell itself in the list of live neighbours
            if (ud == 0 && lr == 0)
              continue;
            // The long expressions in the brackets simply readjust the indices to be within the grid
            // for periodic boundary conditions
            if ( current_grid[r+ud+(r+ud<0)*rows-(r+ud>=rows)*rows][c+lr+(c+lr<0)*cols-(c+lr>=cols)*cols] == 1)
              live_count++;
          }
        }

        // Determine if the cell will be dead or alive in the next generation
        if (current_grid[r][c] == 0 && live_count == 3){
          updated_grid[r][c] = 1;
        } else if (current_grid[r][c] == 1 && ( live_count == 2 || live_count == 3)){
          updated_grid[r][c] = 1;
        } else {
          updated_grid[r][c] = 0;
        }
      }
    }

    // Replace the current grid with the updated
    bool current_grid_unchanged = true;
    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        if ( current_grid[r][c] != updated_grid[r][c])
          current_grid_unchanged = false;
        current_grid[r][c] = updated_grid[r][c];
      }
    }

    // Set a flag in order to determine if cells are dead while printing grid
    bool all_dead = true;
    std::cout << "\033[2J\033[H";
    std::cout<< "Generation: " << generation << "\n"; 
    for (int r = 0; r < rows; r++){
      for (int c = 0; c < cols; c++){
        if (current_grid[r][c] == 0){
          std::cout << "\033[40m-\033[0m";
        } else {
          all_dead = false;
          std::cout << "\033[41m+\033[0m";
        }
      }
      std::cout << "\n";
    }
    if (current_grid_unchanged){
      if (all_dead){
        std::cout << "The population survived for " << generation << " generations before dying out\n";
      } else {
        std::cout << "The population evolved for " << generation << " generations before stabilizing completely\n";
      }
      break;
    }
  }
  return 0;
}

