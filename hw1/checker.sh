check_dir=check_$$

#echo check_$$
if [ "$1" == "" ]; then
    echo "usage: ./checker.sh <fileName>"
    exit 2
fi

tgz_file=$1

echo "Checking tgz file \"$tgz_file\""

if [ ! -f $tgz_file ]; then
	echo "------ERROR-------"
    echo "cannot find .tgz file \"$tgz_file\"!"
    exit 2
fi

if [ !"$tgz_file" == *.tgz ]; then
	echo "------ERROR-------"
    echo "wrong format"
    echo "tgz should be named as \"<stu_id>.tgz\""
    exit 2
fi

echo $tgz_file



mkdir $check_dir
cd $check_dir

tar zxvf ../$tgz_file

name=`basename -s .tgz $tgz_file`
stu_dir=${name}

#echo $name
#echo $stu_dir

if [ ! -d ${stu_dir} ]; then
	echo "------ERROR-------"
    echo "cannot find directory \"${stu_dir}/\""
   	echo "directory should be named as \"<stu_id>\""
	cd ..
    rm -rf $check_dir
    exit 2
fi


readme=${stu_dir}/readme.txt
if [ ! -f ${readme} ]; then
	echo "------ERROR-------"
    echo "cannot find \"${readme}\""
	echo "readme should be named as readme.txt"
    cd ../
    rm -rf $check_dir
    exit 2
fi


bin=${stu_dir}/$name
if [ ! -f ${bin} ]; then
	echo "------ERROR-------"
    echo "cannot find \"${bin}\""
	echo "binary file should be named as \"<stu_id>\""
    cd ../
    rm -rf $check_dir
    exit 2
fi

rm -rf $bin
echo "recompile.."
cd ./$stu_dir
make clean
make
cd ..

if [ ! -f ${bin} ]; then
	echo "------ERROR-------"
    echo "cannot find \"${bin}\" after compile"
	echo "binary file should be named as \"<stu_id>\" after compile"
    cd ../
    rm -rf $check_dir
    exit 2
fi

echo "-------------------PASS-------------------"
cd ../
rm -rf  $check_dir
