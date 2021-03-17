
If you commit a big file (more than 100Mo by default), in this exemple "external/llvm_/bin/clang++", you can't push any more and can be in a blocke situation , to fixe that :  
    'git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch external/llvm_/bin/clang++''  
       (This will remove this big file fro mthe index)  
