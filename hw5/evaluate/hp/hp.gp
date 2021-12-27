reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 378,252 to 1414,714 lw 1 fs empty
set label "clkc" at 896,483 center
set object 2 rect from 0,0 to 378,700 lw 1 fs empty
set label "clkd" at 189,350 center
set object 3 rect from 2394,210 to 3374,420 lw 1 fs empty
set label "cmp1" at 2884,315 center
set object 4 rect from 1414,252 to 2394,462 lw 1 fs empty
set label "cmp2" at 1904,357 center
set object 5 rect from 2394,0 to 3374,210 lw 1 fs empty
set label "cmp3" at 2884,105 center
set object 6 rect from 0,714 to 3304,1260 lw 1 fs empty
set label "cntd" at 1652,987 center
set object 7 rect from 0,1260 to 3304,1806 lw 1 fs empty
set label "cntu" at 1652,1533 center
set object 8 rect from 378,0 to 2394,252 lw 1 fs empty
set label "npd" at 1386,126 center
set object 9 rect from 0,1806 to 3080,2268 lw 1 fs empty
set label "nps" at 1540,2037 center
set object 10 rect from 0,2730 to 2016,2982 lw 1 fs empty
set label "ppd" at 1008,2856 center
set object 11 rect from 0,2268 to 3080,2730 lw 1 fs empty
set label "pps" at 1540,2499 center
set xtics 168
set ytics 168
set size square
set grid  
plot [0:3374][0:3374] 0
set terminal png size 1024,768
set output "hp.png"
replot
set terminal x11 persist size 1024,768
replot
