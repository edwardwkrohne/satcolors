set term pdf size 8,10.5
set output "out.pdf"
set palette defined ( -1 "gray", 0 "white", 1 "black", 2 "blue", 3 "red", 4 "green")
set cbrange [-1:4]
unset label
unset title
unset key
unset colorbox
set xtics 1 format ""
set ytics 1 format ""
set grid front lt 1 lc 8
set size ratio -1
set yrange [] reverse
unset xtics
unset ytics
#set multiplot layout 8,6
set tmargin .3
set bmargin .3
set lmargin .3
set rmargin .3
plot 'output' matrix using (0.5+$1):(0.5+$2):($3) with image
