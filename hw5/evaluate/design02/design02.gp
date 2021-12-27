reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0,150 to 75,275 lw 1 fs empty
set label "B1" at 37,212 center
set object 2 rect from 0,75 to 75,150 lw 1 fs empty
set label "B2" at 37,112 center
set object 3 rect from 0,0 to 150,75 lw 1 fs empty
set label "B3" at 75,37 center
set object 4 rect from 75,75 to 150,225 lw 1 fs empty
set label "B4" at 112,150 center
set object 5 rect from 0,275 to 150,325 lw 1 fs empty
set label "B5" at 75,300 center
set xtics 16
set ytics 16
set size square
set grid  
plot [0:325][0:325] 0
set terminal png size 1024,768
set output "design02.png"
replot
set terminal x11 persist size 1024,768
replot
