#!/usr/bin/perl

use Date::Manip;
use Getopt::Long;
use Data::Dumper;

 Getopt::Long::Configure("bundling");
#
# variabili.
$gnuplot_dir="/home/httpd/html/tsol/gnuplot_descr";
$time_step=15;

my $VERBOSE=0;
my $tot_pkts =0  ;
$opt_l = 0;
$dir = ".";
$opt_n = 0;
$opt_lx = 0;
$opt_ly = 0;


%month = (
	Jan => '01',
	Feb => '02',
	Mar => '03',
	Apr => '04',
	May => '05',
	Jun => '06',
	Jul => '07',
	Aug => '08',
	Sep => '09',
	Oct => '10',
	Nov => '11',
	Dec => '12',
);

sub usage() {
print "
Usage:
     plot_cum.pl <-d dir[,dir]> <-v> [<-l> | [-n] <-f data_file[,data_file]>
                  [-c=col] [-g] [-lx] [-ly] [-xra=s] [-xrb=s] [-yra=s] [-xrb=s]
                  -b \"title\" [-o SS:ss EE:ee]
                  \"aaaa/mm/gg hh:mm\" \"aaaa/mm/gg hh:mm\"]
     
     -l : will produce a list of available traces in the selected dir, sorted by
          date

     -d : directory with the output of tstat
     -f : measure you want to consider
     -n : plot results in % (normalized output)
     -N : plot results in % (cumulated normalized output)
     -v : print verbose debugging information
     -o SS:ss EE:ee : select the time interval span from SS:ss to EE:ee,
        i.e., all traces that do not belong to this time interval will not be
        considered
     -c=col: select the # column in the tstat data file
     -g : generate a .gnu files, suitable to be used by gnuplot
        
   gnuplot options: this options are used in the .gnu file
     -lx: used logarithmic scale on X axis (for gnuplot)
     -ly: used logarithmic scale on Y axis (for gnuplot)
     -xra=s: set starting point of xrange equal to s (for gnuplot)
     -xrb=s: set ending point of xrange equal to s (for gnuplot)
     -yra=s: set starting point of yrange equal to s (for gnuplot)
     -yrb=s: set ending point of yrange equal to s (for gnuplot)
     -b=title: set the title of the plot as \"title\". Generated files will
               be namde title.[cmd,data] as well

     \"aaaa/mm/gg hh:mm\" \"aaaa/mm/gg hh:mm\" defines the initial and ending
        date of the trace that should be considered for the post-processing
        procedure.
        
For example:
      perl plot_cum.pl  -l -d traces/lan/
dumps a sorted list of all traces available from the set stored in traces/lan

perl plot_cum.pl  -d traces/lan \\
     -f protocol_usc \"2002/01/18 23:57\" \"2002/02/19 00:57\" \\
     -v -g -o \"0:00 24:00\" -b foo \\
     
will produce a plot from the files in traces/lan/, considering the
\"protocol_usc\" measure. The time interval considered starts from
\"2002/01/18 23:57\" and ends at \"2002/02/19 00:57\";
Gnuplot filename will be named \"foo.cmd\", \"foo.data\" will contain raw data.

";
	exit();
}


GetOptions ('l'  => \$opt_l, 
	    'd=s'=> \$dir,
	    'n'  => \$opt_n,
	    'N'  => \$opt_N,
	    'lx'  => \$opt_lx,
	    'ly'  => \$opt_ly,
	    'g'  => \$opt_g,
	    'v'  => \$VERBOSE,
	    'b=s'=> \$opt_b,
	    'xra=s'=> \$opt_xra,
	    'xrb=s'=> \$opt_xrb,
	    'yra=s'=> \$opt_yra,
	    'yrb=s'=> \$opt_yrb,
	    'o=s'=> \$opt_o,
	    'f=s'=> \$opt_f) || usage();

@data_files = split(/,/,$opt_f);


$opt_f or $opt_l or usage();
$opt_n = 2 if $opt_N>0;

if ( ! $opt_l) {
     if(! @ARGV ) { 
        
        $all_dates_are_good = 1;
     } else {
	($start_date = shift) ||  usage();
	$s_date=&ParseDate("$start_date") || die "Cannot parse start date \n";

	($end_date = shift) || usage();
	$e_date=&ParseDate("$end_date") || die "Cannot parse end date \n";
     }   
}

if ($opt_o) {
	$opt_o =~ /(\d+):(\d+) (\d+):(\d+)/ || die "Cannot parse -o option\n";
	$s_hour = $1*60+$2;
	$e_hour = $3*60+$4;
}

print "$s_date - $e_date\n" if $VERBOSE;

($#ARGV != -1) && do {
	printf "Cannot parse command line.\n";
	usage();
};

# main

opendir(DIR, $dir) || die "Can't open dir: $dir \n";
@traces = grep { /.out$/ && -d "$dir/$_" } readdir(DIR);
closedir DIR;
($#traces == -1) && die "No trace dirs here: $dir\n";

foreach $trace (@traces) {
	$trace =~ /^(\d\d)_(\d\d)_(\d\d)_(\w\w\w)_(\d\d\d\d)/;
	$trace_date = "$5$month{$4}$3$1:$2:00";
	$traces{$trace_date} = $trace;
	print "$trace - $trace_date\n" if $VERBOSE;
}

#while ( ($key, $value) = each %traces) {
#	print "$key = $value\n";
#}

# fatto questo scorrendo %traces secondo @time_keys
# ho l'elenco delle tracce in ordine di tempo
@time_keys = sort keys %traces;

# se devo solo listare la dir...

($opt_l) && do {
	foreach $trace_date (@time_keys) {
		print &UnixDate($trace_date,"%Y/%m/%d %H:%M %a") . "\n";
	}
	exit();
};


# qui invece faccio lavoro sporco
$data_file = $data_files[0];
foreach $data_file (@data_files) {
    foreach $trace_date (@time_keys) {
		opendir(DIR, "$dir/$traces{$trace_date}") || die "can't opendir $dir/$traces{$trace_date}: $!";
		@qdirs = grep { /\d\d$/ && -d "$dir/$traces{$trace_date}/$_" } readdir(DIR);
		closedir DIR;
		foreach $q  (@qdirs) {
			$step = $q * $time_step;
			$q_date=&DateCalc($trace_date,"+ " . $step . "minutes");
			if ( trace_is_good($q_date) ) {
				$i=0;
                                $plain = "$dir/$traces{$trace_date}/$q/$data_file";
				open IN, (-e $plain) ? "$plain" : "gzip -dc $plain.gz |" 
                                             or die "Can't open file: $plain";
                                while (<IN>) {
                                        chomp;
                                        if( m/^#min=(\d+)\s*bin_size=(\d+)\s*max=(\d+)\s*/ ) {
                                           ($min[$d],$delta[$d],$max[$d])=  ($1,$2,$3);
                                           next;
                                        } elsif( m/^#/ ) {
                                           (  $title[$d] = $_ ) =~ s/^#//;
                                           next;
                                        }
					@line = split;
					$val[$d]{$line[0]} += $line[1];
					$totali[$d] += $line[1];
				}
				close IN;
				print "Using $plain \n" if $VERBOSE;
			} else {
				print "Rejecting $dir/$traces{$trace_date}/$q/ \n" if $VERBOSE;
			}
		}
	}
	$d++;
	print "Totali per $d = $totali[$d]\n" if $VERBOSE;
}

if (! $opt_b) {
	$opt_b = $data_files[0];
}


if($opt_g) {
	open OUT, ">$opt_b.data";
} else {
	open OUT, ">&STDOUT";
}

# in $d ho il nmero di files esaminati

foreach ($k=0; $k<$d; $k++) {
        while($min[$k] <=$max[$k]) {
           $val[$k]{$min[$k]} ||= 0;
           $min[$k] += $delta[$k];
        }
        $CDF=0;
	foreach $key (sort { $a <=> $b } keys  %{ $val[$k] } )  {
             print OUT  "$key $val[$k]{$key} ",  $pdf = $val[$k]{$key} / ($opt_n ? $totali[$k] : 1.0), " " , $CDF += $pdf, "\n";  
        }   
}


if ($opt_g) {
	# Output per gnuplot
        
        # vediamo se c'e' un file di modello da includere...
        
        @predef=split(',',$opt_f);

        if (-e "$gnuplot_dir/$predef[0]")
        {
                system("cp $gnuplot_dir/$predef[0] $opt_b.cmd");
        	open G_OUT, ">>$opt_b.cmd" || warn "Cannot open file $opt_b.cmd";
        }
        else
        {
        	open G_OUT, ">$opt_b.cmd" || warn "Cannot open file $opt_b.cmd";
        	print G_OUT "set data style steps\n";
        }
       	print G_OUT "set terminal png small; set output '$opt_b.png'\n";
        

	if ($opt_xra) {
		print G_OUT "set xrange [ $opt_xra :]\n";
	}
	if ($opt_xrb) {
		print G_OUT "set xrange [ :$opt_xrb]\n";
	}
        
	if ($opt_yra) {
		print G_OUT "set yrange [ $opt_yra :]\n";
	}
	if ($opt_yrb) {
		print G_OUT "set yrange [ : $opt_yrb]\n";
	}

	if ($opt_n) {
		print G_OUT "set ylabel '%'\n";
	}
        else {
		print G_OUT "set ylabel 'hits'\n";
        }

$opt_lx && print G_OUT "set logscale x\n";
$opt_ly && print G_OUT "set logscale y\n";

	print G_OUT "\nplot ";
	for ($k=1; $k<$d; $k++) {
		print G_OUT "'$opt_b.data' index @{[ $k-1 ]} using 1:(\$" . (2+$opt_n) . ") title \"$title[$k-1]\", \\\n";
	}
		print G_OUT "'$opt_b.data' index @{[ $k-1 ]} using 1:(\$" . (2+$opt_n) . ") title \"$title[$k-1]\" \n";
	close G_OUT;
}

sub trace_is_good() {
	# questa funzione mi dice se la traccia ha una data (o un'ora) che mi piace.

	$q_date = shift;

	#if ((&Date_Cmp($s_date,$q_date) <= 0) && (&Date_Cmp($q_date,$e_date) < 0)) {
	if ( $all_dates_are_good || (($s_date le $q_date) && ($q_date le $e_date))) {
		if (!$opt_o) {
			return 1;
		} else {
			print "$q_date" if $VERBOSE;
			$q_date =~ /(\d\d):(\d\d):\d\d$/;
			$q_hour = $1*60+$2;
			print " $q_hour\n" if $VERBOSE;
			if (($q_hour>=$s_hour) && ($q_hour<$e_hour)) {
				return 1;
			} else {
				return 0;
			}
		}
	} else {
		return 0;
	}	
}
