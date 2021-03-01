#include <stdio.h>
#include <emscripten.h>
EM_JS(char, get_character, (), {
    return Module.inputAsync();
})
EM_JS(void, ff_exit, (), {
    return Module.ff_exit();
})
 int main(int argc, char* argv[]) {
  
    if(argc!=2){
      printf("invalid arguments\n");
      ff_exit();
      return 1;
    }
    char * fileName =argv[1];
    emscripten_wget( fileName, fileName );
    FILE *file = fopen(fileName, "rb");
    if (!file) {
      printf("cannot open file\n");
      ff_exit();
      return 1;
    }
    while (!feof(file)) {
      char c = fgetc(file);
      if (c != EOF) {
        putchar(c);
      }
    }
  fclose (file);
  ff_exit();
}
