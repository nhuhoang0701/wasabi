
// Don't delete the retruned char *
// Not threat safe: so calls all theses methods in the same theard or at least in a sequantial way.
extern "C" const char* json_getServerInfo();
extern "C" const char* json_getResponse_json(const char* InA);
