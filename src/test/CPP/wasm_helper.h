
#define EXPORT __declspec(dllexport)
#if !defined(WASM_HELPER)
#define WASM_HELPER
/*

void *__cxa_allocate_exception(size_t thrown_size);
void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *));
EXPORT void* __cxa_begin_catch(void* unwind_arg);
EXPORT void __cxa_end_catch();

*/
extern "C"
{
    void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *))
    {
        ;
    }
    
    void *__cxa_allocate_exception(size_t thrown_size) throw()
    {
        return NULL;
    }
    
    void* __cxa_begin_catch(void* unwind_arg) throw()
    {
        return unwind_arg;
    }
    void __cxa_end_catch() throw()
    {

    }
	
}
#endif // WASM_HELPER