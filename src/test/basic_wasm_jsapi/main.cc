#define EXPORT __attribute__ ((visibility ("default")))

extern "C"
{
  extern void js_console_log_str(const char*);

  EXPORT void tolower(char* str)
  {  
	js_console_log_str("tolower from C");
	js_console_log_str(str);
	
	char* index= str;
	while( *index != 0)
	{
		(*index) += 32;
		index++;
	}
	
	js_console_log_str(str);
  }
}
