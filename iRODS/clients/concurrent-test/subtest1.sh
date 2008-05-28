#!/bin/sh -ex

#####################################################################################
#  test1, uses some of the srb commands, mostly on srb Ojects in order to see if the
#  commands are all working according to their design purpose.
#  
#  Unless otherwise stated, all commands with # in front of them, don't seem to be 
#  working properly.
#
#  By: Arcot Rajasekar
#  Modified By: Roman Olschanowsky
#  And by: Susan Lindsey for iRODS
#####################################################################################

OS=Darwin
storeset=`cat ResourcesToTest`


# SDL get rid of hardcoding
irodshome=/Users/slinz/iRODS


iDir=$irodshome/clients/icommands/bin
DATA_FILES="data_1 data_2 data_3 data_4 datadb1.xml dataempty"
TMP_DIR=../TMP

#####################################################################################
#  Make unique names for directories using current date and time
#####################################################################################

if [ "$1" = "" ]
then testid=`date "+%Y%m%d%H%M%S"`
else testid=$1
fi

testdate=`date "+%Y-%m-%d"`
echo "TESTID = $testid"
echo "TESTDATE = $testdate"

#####################################################################################
#  Initialize srb client environment, and get User information
#####################################################################################
#$iDir/iinit -v
$iDir/iuserinfo -v
$iDir/ilsresc -vl

#####################################################################################
#  Make new test directory in irods space, and get meta attributes
#####################################################################################


$iDir/imkdir Itest1Dir$testid
$iDir/icd Itest1Dir$testid
for store1 in $storeset ; do

	echo "Testing puts and gets in resource: " $store1

  $iDir/imkdir Isub1
  $iDir/ils -lr
  $iDir/icd Isub1


#####################################################################################
#  Put local data files into newly created directories in irods space, do some 
#  description of file annotation, then modify some meta data.
#####################################################################################
  for file in $DATA_FILES ; do
      $iDir/iput -f -R $store1 $file 
#      $iDir/iannotate -w page1 "Test Data Stuff" $file
#      $iDir/iannotate -Y 0 $file
#      $iDir/imodD -t text $file
  done


  $iDir/icd ..

#####################################################################################
#  Display various data information about objects in irods space
#####################################################################################
  $iDir/iget -rf Isub1
  $iDir/ils -lr


#####################################################################################
#  Display and Get all data files from srb space, store them in temp directory, 
#  and compare them with another copy of same file to see if they match.
#####################################################################################
  for file in $DATA_FILES ; do
#      $iDir/Scat S_sub1/$file
      $iDir/iget -rf Isub1/$file $TMP_DIR/$file.$testid.tmp
      diff $file $TMP_DIR/$file.$testid.tmp
      wc $file $TMP_DIR/$file.$testid.tmp
      rm $TMP_DIR/$file.$testid.tmp
  done

  $iDir/imkdir Isub2

#####################################################################################
#  Move all files from Sub1 to Sub2
#####################################################################################
  for file in $DATA_FILES ; do
      $iDir/imv Isub1/$file Isub2
  done

  $iDir/ils -lr

#####################################################################################
#  Copy all data across different platforms, replicate, then get data to local 
#  environment, and compare with local copies of same data.
#####################################################################################
  for store in $storeset ; do
    for file in $DATA_FILES ; do
       $iDir/icp -f -R $store Isub2/$file Isub1/$file.$store
# SDL what's going on here?
#       $iDir/irepl -R $store Isub1/$file.$store
    done

    $iDir/ils -lr
    $iDir/iget -rf Isub1
    $iDir/iget -rf Isub2

    for file in $DATA_FILES ; do
      $iDir/iget Isub1/$file.$store $TMP_DIR/$file.$testid.tmp
      diff $file $TMP_DIR/$file.$testid.tmp
      wc $file $TMP_DIR/$file.$testid.tmp
      rm $TMP_DIR/$file.$testid.tmp
    done
  done

  $iDir/icd Isub2
  $iDir/icd ..



#####################################################################################
#  copy data_1 to susan, then append all data files to susan file and cat it
#####################################################################################
# SDL don't think we need this
#  $iDir/icp -R $store data_1 susan
#  for file in $DATA_FILES ; do
#     $iDir/iappend -s $file susan
#  done
#  $iDir/icat susan
#  $iDir/irm susan
#  $iDir/icd ..

#####################################################################################
#  Do all the clean up, remove data files and directories
#####################################################################################

  $iDir/irm -r Isub1
  $iDir/irm -r Isub2
  $iDir/ils -lr
done

#####################################################################################
#  Remove test directory and end session
#####################################################################################

$iDir/ipwd
# SDL need to change this
$iDir/icd  /susanzone/home/rods
$iDir/ipwd
$iDir/irm -r Itest1Dir$testid
$iDir/irmtrash
$iDir/iexit
exit 0

