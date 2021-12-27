reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 150,75 to 225,200 lw 1 fs empty
set label "B1" at 187,137 center
set object 2 rect from 0,0 to 75,75 lw 1 fs empty
set label "B2" at 37,37 center
set object 3 rect from 0,75 to 150,150 lw 1 fs empty
set label "B3" at 75,112 center
set object 4 rect from 75,0 to 225,75 lw 1 fs empty
set label "B4" at 150,37 center
set object 5 rect from 0,150 to 150,200 lw 1 fs empty
set label "B5" at 75,175 center
set xtics 11
set ytics 11
set size square
set grid  
plot [0:225][0:225] 0
set terminal png size 1024,768
set output "design01.png"
replot
set terminal x11 persist size 1024,768
replot
