# PS4: Sokoban

## Contact
Name: Jarrett Williams
Section: 201
Time to Complete: 6 days 


## Description
Explain what the project does.

the project is to draw the base of a game of sokoban

### Features
Describe what your major decisions were and why you did things that way.

#### Part a

nothing really special I decided to use a 2D vector to store the multiple textures and print them as sprites. within the draw virtual function I created two 
nested for loop that takes the size of the 2D vector. the first one draws the ground the second one draws anything ontop of it. so the character, crates ground and the crate location where the crates should go for the game. The main window has to be size appropritate to the play area so I used the pixelHeight and pixelwidth for its window dimensions. 

#### Part b

this part was the most easiest and the most difficult. so for movement I did a switch statment for both the player and the crate. I had to do the collision so the crate will react to the direciton of the player if the player collides with the crate. for reset I had to make another vector that is the inital level. this keeps the data of the inital level and so we can recall that for when we have to reset the level. for the win function I had to make 3 mutliple conditions. for drawing I mad a map that houses all the textures that will be used in the game.

#### changes

for my resumbision I fixed the isWon function 

bool SB::Sokoban::isWon() const {
  if (m_crate_locations.size() == m_crate_spots_locations.size()) {
    for (const auto &crate : m_crate_locations) {
      if (std::find(m_crate_spots_locations.begin(),
                    m_crate_spots_locations.end(),
                    crate) == m_crate_spots_locations.end()) {
        return false;
      }
    }
  }

  if (m_crate_locations.size() <= m_crate_spots_locations.size()) {
    std::cout << "there are more targets than crates" << std::endl;
    for (const auto &crate : m_crate_locations) {
      if (std::find(m_crate_spots_locations.begin(),
                    m_crate_spots_locations.end(),
                    crate) == m_crate_spots_locations.end()) {
        return false;
      }
    }
  }
  if (m_crate_locations.size() > m_crate_spots_locations.size()) {
    std::cout << "theres more crates than targets!" << std::endl;
    for (const auto &targets : m_crate_spots_locations) {
      if (level_generation[targets.y][targets.x] != 'A') {
        return false;
      }
    }
  }
  if (m_crate_locations.size() == 0 || m_crate_spots_locations.size() == 0) {
    return true;
  }
  m_direction = Direction::Down;
  std::cout << "YOU WON!" << std::endl;
  return true;
}

I used the algorithim function find to find the amount of targets to see if any crates are populated in them. if not then you dont win if you do then you'll win also for the last if statment checks if theres more crates than targets, if so then if all targets have crates in them you'll win the game

also in the movePlayer function I had to do this

for (auto &loc : m_crate_locations) {
    if (loc == new_pos) {
      loc = crates_pos;
      break;
    }
  }

this search loop will keep updating the position of the crates so we can use it for the win function while we find it 

I also tweaked the undo function where the crates instead of going back to its inital spot will go back each spot it was moved just like the character

also I forgot to mention that the character moves fast so I had to slowdown the framerate by 10 so playing the game wont be a hassel

also I fixed the file input so that it shoudl load in the files

### Memory
Describe how you decided to store the level data including whether you used smart pointers.

I stored the level data into a 2D vector.
I used maps for loading in the textures. ngl I copied what prof.Daly did for the snake game we did in class and how he made the snake change sprite depending on the direciton

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.

I didn't use any algoritm functions but I used alot of vectors and maps for textures and the matrixes
the only lambda expression I decided to do is for the movement of the charcter it woks just as good if I did a normal switch statment

### Lambdas and alogrithm changes
the only diference I did was that I used the find function

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

The real thing that I had trouble with was the positing of everything. for some reason when I set the position of the sprites it would either b shifted by one TILE_SIZE on the top or left or right. it wasnt until I had to minus i * TILE_SIZE by a value to get the desired output so I did (i * TILE_SIZE) - TILE_SIZE and got the right output and getting the character position which I had to make a psudeo vector to get the line I wanted and used it to get the exact position of the character. 

I had issues with reading the inital position of the level for the character position. It was defenitly how I was reading the character so what I did is instead of pushing the read level into the level_generation 2D vector and before that resizing the 2D vector with the already read height and width



### Extra Credit
Anything special you did.  This is required to earn bonus points.

I added some code to do the elapsed time for the duration of the game.
I made it so the character would change the direciton of the sprite depending on the players moveemnt
I also added an undo function that will undo to the point of the players and crates inital position at the start of gameplay


## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.

Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban

font is BNJinx
