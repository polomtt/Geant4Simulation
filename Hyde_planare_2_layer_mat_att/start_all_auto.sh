cd build
make
rm efficenza

for spes_li6 in 0 1 2 5 10 15 20 25 30 40 50 100; do
        for spes_b10 in 0 1 2 5 10 15 20 25; do
                        echo $spes_b10 $spes_li6 > dimension_act_mat
                        ./exampleB1 run_neutron.mac
                        root -q ../root_analysis.cxx
                        rm -r *.root
        done;
done;

cd ..