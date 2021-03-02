
Before delivers :
   for the moment make a review validation with the owner (Ghislain HUDE) throw pull request
   verify that the 'rebuild' command works from :
      the folder of your componant
      from the wasabi root dir (folder which contains README.md, src/, scripts/)

Structure project:
  Each components (aka dbproxy, InA_interpreter, etc..) should follow a flat structure (CMakeLists.txt, header and source at the same level)
      Once the final target/structure will be define we we do more complexe/structure 
      Only resources folders can be created  to store resources needed at runtimeat the same level of the CMakeLists.txt.
  Each component should have real test (with CPPUNIT like assert ) to verify that the compnent work 
      Each test should simple as possible and test only the API of the component and reduce usage of other component.

Don't log too much in the console, also for test, try to comment your logs for debuging before deliver to don't poluate the console.
    We didn't have logger for the moment ;(

