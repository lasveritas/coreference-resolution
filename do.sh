rm tes/names.txt
rm tes/misc.txt
touch tes/names.txt
touch tes/misc.txt

cat $1 | ~/abbyy/yandex/tomita-parser/build-debug/bin/tomita-parser config_raw.proto
python3 name.py
cat $1 | ~/abbyy/yandex/tomita-parser/build-debug/bin/tomita-parser config_strict_xml.proto
#cat $1 | ~/abbyy/yandex/tomita-parser/build-debug/bin/tomita-parser config_strict_txt.proto

python3 clusterize.py clusters.txt

rm *bin
