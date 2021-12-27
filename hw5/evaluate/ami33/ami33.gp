reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 350,819 to 686,952 lw 1 fs empty
set label "bk1" at 518,885 center
set object 2 rect from 0,700 to 378,819 lw 1 fs empty
set label "bk10a" at 189,759 center
set object 3 rect from 203,1071 to 364,1211 lw 1 fs empty
set label "bk10b" at 283,1141 center
set object 4 rect from 0,1813 to 119,1862 lw 1 fs empty
set label "bk10c" at 59,1837 center
set object 5 rect from 175,819 to 350,938 lw 1 fs empty
set label "bk11" at 262,878 center
set object 6 rect from 0,84 to 140,490 lw 1 fs empty
set label "bk12" at 70,287 center
set object 7 rect from 546,0 to 686,497 lw 1 fs empty
set label "bk13" at 616,248 center
set object 8 rect from 350,140 to 546,259 lw 1 fs empty
set label "bk14a" at 448,199 center
set object 9 rect from 140,350 to 434,469 lw 1 fs empty
set label "bk14b" at 287,409 center
set object 10 rect from 504,1071 to 665,1190 lw 1 fs empty
set label "bk14c" at 584,1130 center
set object 11 rect from 378,1421 to 497,1687 lw 1 fs empty
set label "bk15a" at 437,1554 center
set object 12 rect from 0,1421 to 119,1757 lw 1 fs empty
set label "bk15b" at 59,1589 center
set object 13 rect from 84,1071 to 203,1197 lw 1 fs empty
set label "bk16" at 143,1134 center
set object 14 rect from 0,1211 to 371,1393 lw 1 fs empty
set label "bk17a" at 185,1302 center
set object 15 rect from 511,497 to 693,700 lw 1 fs empty
set label "bk17b" at 602,598 center
set object 16 rect from 0,497 to 182,700 lw 1 fs empty
set label "bk18" at 91,598 center
set object 17 rect from 0,1071 to 84,1190 lw 1 fs empty
set label "bk19" at 42,1130 center
set object 18 rect from 245,1421 to 378,1715 lw 1 fs empty
set label "bk2" at 311,1568 center
set object 19 rect from 497,1421 to 679,1771 lw 1 fs empty
set label "bk20" at 588,1596 center
set object 20 rect from 140,0 to 455,140 lw 1 fs empty
set label "bk21" at 297,70 center
set object 21 rect from 378,497 to 511,812 lw 1 fs empty
set label "bk3" at 444,654 center
set object 22 rect from 0,1862 to 560,1995 lw 1 fs empty
set label "bk4" at 280,1928 center
set object 23 rect from 182,497 to 315,637 lw 1 fs empty
set label "bk5a" at 248,567 center
set object 24 rect from 0,819 to 175,952 lw 1 fs empty
set label "bk5b" at 87,885 center
set object 25 rect from 504,1190 to 637,1421 lw 1 fs empty
set label "bk5c" at 570,1305 center
set object 26 rect from 371,1071 to 504,1386 lw 1 fs empty
set label "bk6" at 437,1228 center
set object 27 rect from 245,1715 to 427,1813 lw 1 fs empty
set label "bk7" at 336,1764 center
set object 28 rect from 140,140 to 350,350 lw 1 fs empty
set label "bk8a" at 245,245 center
set object 29 rect from 119,1421 to 245,1799 lw 1 fs empty
set label "bk8b" at 182,1610 center
set object 30 rect from 504,952 to 686,1071 lw 1 fs empty
set label "bk9a" at 595,1011 center
set object 31 rect from 371,952 to 490,1071 lw 1 fs empty
set label "bk9b" at 430,1011 center
set object 32 rect from 0,952 to 357,1071 lw 1 fs empty
set label "bk9c" at 178,1011 center
set object 33 rect from 0,0 to 119,84 lw 1 fs empty
set label "bk9d" at 59,42 center
set xtics 99
set ytics 99
set size square
set grid  
plot [0:1995][0:1995] 0
set terminal png size 1024,768
set output "ami33.png"
replot
set terminal x11 persist size 1024,768
replot
