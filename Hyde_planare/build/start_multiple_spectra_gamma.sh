Help()
{
    # Display Help
    echo "Select source for simulation:"
    echo "  gamma source --> 1"
}

while getopts ":h" option
do
   case "${option}" in
        h) # display Help
            Help
            exit;;
   esac
done

TODAY="`date "+%Y%m%d_%H%M"`"
rm efficenza_gamma
echo "energy;eff" > efficenza_gamma

for i in $(seq 10 50 2000);
    do echo "Simulo $i keV"

        python3 create_mac_file_gamma.py $i
        echo "Uso Gamma"
        run_file="run_multiple_gamma.mac"

        set -e
        echo "Compilo"
        make -j3
        echo "Inizio simulazione"
        ./exampleB1 $run_file >/dev/null

        echo "Disegno grafici"
        root -l -q root_analysis_gamma.cxx
        
#         cp histo.root histo_for_efficency/histo_${i}_keV.root
#         cp histo.png histo_for_efficency/histo_${i}_keV.png
#         rm histo.png
#         rm histo.root
#         rm run_multiple_gamma.mac

done
