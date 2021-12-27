reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 2590,3024 to 3885,3640 lw 1 fs empty
set label "BLKB" at 3237,3332 center
set object 2 rect from 2590,0 to 3885,490 lw 1 fs empty
set label "BLKD" at 3237,245 center
set object 3 rect from 2590,490 to 3885,3024 lw 1 fs empty
set label "BLKLL" at 3237,1757 center
set object 4 rect from 0,0 to 1295,2569 lw 1 fs empty
set label "BLKLR" at 647,1284 center
set object 5 rect from 0,2569 to 756,3409 lw 1 fs empty
set label "BLKP" at 378,2989 center
set object 6 rect from 0,3640 to 1162,5579 lw 1 fs empty
set label "BLKRC" at 581,4609 center
set object 7 rect from 2457,3640 to 3675,5292 lw 1 fs empty
set label "BLKRS" at 3066,4466 center
set object 8 rect from 1295,2114 to 2177,3430 lw 1 fs empty
set label "BLKT" at 1736,2772 center
set object 9 rect from 1295,0 to 2590,2114 lw 1 fs empty
set label "BLKUL" at 1942,1057 center
set object 10 rect from 1162,3640 to 2457,5579 lw 1 fs empty
set label "BLKUR" at 1809,4609 center
set xtics 278
set ytics 278
set size square
set grid  
plot [0:5579][0:5579] 0
set terminal png size 1024,768
set output "xerox.png"
replot
set terminal x11 persist size 1024,768
replot
