#include <iostream>
#include <dirent.h>
#include <emscripten.h>
EM_JS(char, get_character, (), {
    return Module.inputAsync();
})
EM_JS(void, ff_exit, (), {
    return Module.ff_exit();
})
int listdir(const char *path) 
{
  struct dirent *entry;
  DIR *dp;

  dp = opendir(path);
  if (dp == NULL) 
  {
    std::cerr <<"opendir" << std::endl;
      ff_exit();
    return -1;
  }

  while((entry = readdir(dp)))
    std::cout << (entry->d_name) << std::endl;

  closedir(dp);
    ff_exit();
  return 0;
}

int main(int argc, char **argv) {
  
   std::cout << "C Program for listing directories" << std::endl << std::endl;
  if (argc == 1){
    std::cout << "Listing current directory:" << std::endl;
    listdir(".");
  }
  int counter = 1;
  while (++counter <= argc) {
    std::cout << "\nListing " << argv[counter-1] << std::endl;;
    listdir(argv[counter-1]);
  }
  ff_exit();
}