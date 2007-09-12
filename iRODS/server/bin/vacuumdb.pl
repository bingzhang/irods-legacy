#!/usr/bin/perl
#
# Copyright (c), The Regents of the University of California            ***
# For more information please refer to files in the COPYRIGHT directory ***
#
# This runs a postgresQL vacuumdb, if the iRODS system has not been
# active recently, stopping and restarting the iRODS server and
# agents, and logging activity.  It is normally invoked via an iRODS
# rule but can run by hand too.

sub logMsg {
    $date=`date`;
    chomp($date);
    my($inpText) = @_;
    $_ = $date . " " . $inpText . "\n";
    print F;
    $text = $date . " " . $inpText . "\n";
    print $text;
}

# Adjustable parameters:
$sleepSecs=70;
$sleepRetries=10;
$DB_NAME="ICAT";
$minutesInactive=4;  # minutes iRods must be inactive

#
# Set $myDir the directory containing this script.
# This is used in determining the paths of other scripts, programs,
# and log files.  
#
$startDir=`pwd`;
chomp($startDir);
if (index($0, "/") eq 0) {
    $tmp= $0;
}
else {
    $tmp=$startDir . "/" . $0;
    chomp($tmp);
}
$i = rindex($tmp, "/");
$myDir = substr($tmp, 0, $i);


# Normally, you need not change following values, but you may if you
# didn't use the install.pl script or you moved this script.
$stopScript="$myDir/stop.pl";
$startScript="$myDir/start.pl";
$logDir="$myDir/../log";
$postgresBin="$myDir/../../../iRodsPostgres/pgsql/bin";
$myLogFile="$logDir/vacuumdb.log";

# Open the log file
`touch $myLogFile`;
open(F, ">>$myLogFile");
logMsg "vacuumdb.pl started, pid=$$";

# Check on iRODS activity, waiting until quiet, sleep and try a few
# times.  Quiet means that no clients have connected or disconnected
# in the last $minutesInactive minutes, and no agents (connected
# clients) are currently active.
$uname=`uname -s`;
chomp($uname);
if ($uname eq "Darwin") {
    $psOpts="-ax";
}
else {
    $psOpts="-el";
}

# Check for another copy of this running.
# $$ is the perl pid for this script.
$selfs=`ps -auxg | grep vacuumdb.pl | grep perl | grep -v grep | grep -v $$`;
chomp($selfs);
if ($selfs) {
    logMsg("Another copy of this script is running:");
    logMsg($selfs);
    logMsg("exiting");
    close F;
    exit(-2);
}


$active="1";
for ($i=0;$i<$sleepRetries && $active ;$i++) {
    $logLine = `find $logDir -mmin -$minutesInactive | grep rodsLog`;
    chomp($logLine);
    $agents=`ps $psOpts | grep irodsAgent | grep -v grep | grep -v defunct`;
    chomp($agents);
    if ($logLine) {
	logMsg "active recently";
    }
    if ($agents) {
	logMsg "active now";
    }
    if ($logLine || $agents) {
    }
    else {
	logMsg "inactive";
	$active="";
    }
    if ($active) {sleep($sleepSecs);}
}

if ($active) {
    logMsg("Still active, skipping vacuumdb");
    logMsg "vacuumdb.pl exiting";
    close F;
    exit(-1);
}

#
# Stop the iRODS server
#
logMsg "Stopping the iRODS server, etc";
`yes | $stopScript`;

#
# Sleep a bit to let postgres cleanup
#
logMsg "Sleeping a couple seconds";
sleep(2);

#
# Run the vacuumdb
#
$DB_NAME="ICAT";
chdir("$postgresBin");
$nowDir=`pwd`;
chomp($nowDir);
logMsg "cd'ed to " . $nowDir;
logMsg "Running vacuumdb";
#`$postgresBin/vacuumdb -f -z $DB_NAME`;
`./vacuumdb -f -z -q -d $DB_NAME`;
$status=$?;
if ($status) {
    logMsg "vacuumdb failed";
}

#
# Restart the iRODS server
chdir "$myDir";
$nowDir=`pwd`;
chomp($nowDir);
logMsg "cd'ed to " . $nowDir;

logMsg "Restarting iRODS servers";
`$startScript`;

logMsg "vacuumdb.pl exiting";
close F;


