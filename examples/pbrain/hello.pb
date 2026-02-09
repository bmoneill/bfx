>>
>+++++++[<++++++++++>-]<++  72 (H) on cell #3
>>+++++++[<++++++++++>-]<-  69 (E) on cell #4
>>++++++++[<++++++++++>-]<----  76 (L) on cell #5
>>++++++++[<++++++++++>-]<-  79 (O) on cell #6
>>+++[<++++++++++>-]<++ 32 (space) on cell #7
>>+++++++++[<++++++++++>-]<--- 87 (W) on cell #8
>>++++++++[<++++++++++>-]<++ 82 (R) on cell #9
>>+++++++[<++++++++++>-]<-- 82 (D) on cell #10

[<]< move to cell 1
+( start procedure 1
>>.>.>..>. print HELLO
[<]< return to cell 1
)

+( start new procedure
>>>>>>. print space
[<]< return to cell 1
)

+( start new procedure
>>>>>>>.<<.>>>.<<<<.>>>>>. print WORLD
[<]< return to cell 1
)

+( start new procedure
>>[>] move to next empty cell
++++++++++. print newline
[<]< return to cell 1
)

[-] clear cell 1
+: print HELLO
+: print space
+: print WORLD
+: print newline
