#!/usr/bin/perl

use Date::Manip;
use Getopt::Long;
use Data::Dumper;

# variabili.
$gnuplot_dir="/usr/local/bin/gnuplot_descr";
$time_step=15;

$VERBOSE =0;
$opt_c=1;
$tot_pkts =0  ;
$opt_l = 0;
$dir = ".";
$opt_f = "ac_protocol";
$opt_n = 0;
$opt_lx = 0;
$opt_ly = 0;

$d=0;

sub usage() {
print "
Usage:
     plot_time.pl <-d dir[,dir]> <-v> [<-l> | [-n] <-f data_file[,data_file]>
                  [-c=col] [-g] [-lx] [-ly] [-xra=s] [-xrb=s] [-yra=s] [-xrb=s]
                  -b \"title\" [-o SS:ss EE:ee]
                  \"aaaa/mm/gg hh:mm\" \"aaaa/mm/gg hh:mm\"]
     
     -l : will produce a list of available traces in the selected dir, sorted by
          date

     -d : directory with the output of tstat
     -f : measure you want to consider
     -n : print results in % (normalized output)
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
      perl plot_time.pl  -l -d traces/lan/
dumps a sorted list of all traces available from the set stored in traces/lan

perl plot_time.pl  -d traces/lan \\
     -f protocol_out \"2002/01/18 23:57\" \"2002/02/19 00:57\" \\
     -v -g -o \"0:00 24:00\" -b foo \\
     
will produce a time plot from the files in traces/lan/, considering the
\"protocol_out\" measure. The time interval considered starts from
\"2002/01/18 23:57\" and ends at \"2002/02/19 00:57\";
Gnuplot filename will be named \"foo.cmd\", \"foo.data\" will contain raw data.

";
exit();
}


GetOptions ('l'  => \$opt_l, 
	    'd=s'=> \$dir,
	    'n'  => \$opt_n,
	    'lx'  => \$opt_lx,
	    'ly'  => \$opt_ly,
	    'xra=s'=> \$opt_xra,
	    'xrb=s'=> \$opt_xrb,
	    'yra=s'=> \$opt_yra,
	    'yrb=s'=> \$opt_yrb,
	    'g'  => \$opt_g,
	    'v'  => \$VERBOSE,
	    'o=s'=> \$opt_o,
	    'b=s'=> \$opt_b,
	    'c=s'=> \$opt_c,
	    'f=s'=> \$opt_f) || usage();

@data_files = split(/,/,$opt_f);
@trace_dirs = split(/,/,$dir);

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

($#ARGV != -1) && do {
	printf "Cannot parse command line. (Saranno gli apici attorno alle date ?)\n";
	usage();
};


# main

foreach my $dir (@trace_dirs) {
	undef @traces;
	opendir(DIR, $dir) || die "Can't open dir: $dir \n";
	@traces = grep { /.out$/ && -d "$dir/$_" } readdir(DIR);
	foreach my $trace (@traces) {
		$trace =~ /^(\d\d)_(\d\d)_(\d\d)_(\w\w\w)_(\d\d\d\d)/;
		$trace_date=&ParseDate("$3/$4/$5 $1:$2") || die "Cannot parse date \n";
		$traces{$trace_date} = $trace;
		$trace_dir{$trace_date} = $dir;
	}
	closedir DIR;
}
($#traces == -1) && die "No trace dirs here: $dir\n";

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
$maxindex = 0;
foreach $data_file (@data_files) {
	$qnum=0;
    foreach $trace_date (@time_keys) {
		my $dir = $trace_dir{$trace_date};
		opendir(DIR, "$dir/$traces{$trace_date}") || die "can't opendir $dir/$traces{$trace_date}: $!";
		@qdirs = sort { $a <=> $b } grep { /\d\d$/ && -d "$dir/$traces{$trace_date}/$_" } readdir(DIR);
		closedir DIR;
		foreach $q  (@qdirs) {
			$step = $q * $time_step;
			$q_date=&DateCalc($trace_date,"+ " . $step . "minutes");
			#if ((&Date_Cmp($s_date,$q_date) <= 0) && (&Date_Cmp($q_date,$e_date) < 0)) {
                	#if (($s_date lt $q_date) && ($q_date lt $e_date)) {
                        $result = trace_is_good($q_date);
			if ( $result == 2) { #ora e giorno corretti
				$date[$qnum] = $q_date;
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
					$val[$d][$qnum]{$line[0]} = $line[1];
					$totali[$qnum] += $line[1];
   				   }
   				   close IN;
			        $tot_pkts += $totali[$qnum];
			        $qnum++;
                         } else { #solo giorno corretto, ma ora errata
                            if ( $result == 1) {
                                    print "Zeroing $dir/$traces{$trace_date}/$q/ \n" if $VERBOSE;
                                    $val[$d][$qnum]{0} = "NaN";
                                    $totali[$qnum] = -1;
                                    $qnum++;
                            }
                         }
                        # giorno e ora non corretti
			}
	}
	$d++;
}


# in $d ho il nmero di files esaminati

if($opt_g) {
	open OUT, ">$opt_b.data";
} else {
	open OUT, ">&STDOUT";
}


foreach ($k=0; $k<$d; $k++) {
    foreach ($j=0; $j<$qnum; $j++) {
        $min=$min[$k];
        while( $min<=$max[$k]) {
           print OUT  defined $val[$k][$j]{$min}  ?
             $val[$k][$j]{$min} / ($opt_n ? $totali[$j] : 1.0) : "NaN", " " ;  
           $min += $delta[$k];
        }   
        print OUT "\n";
        }
}

if($opt_g) {
      # creo il cmd file per gnuplot
      @predef=split(',',$opt_f);
      if (-e "$gnuplot_dir/$predef[0]")
      {
              system("cp $gnuplot_dir/$predef[0] $opt_b.cmd");
              open OUT, ">>$opt_b.cmd" || warn "Cannot open file $opt_b.cmd";
      }
      else
      {
              open OUT, ">$opt_b.cmd" || warn "Cannot open file $opt_b.cmd";
              print OUT "set terminal png small; set output '$opt_b.png'\n";
              print OUT "set data style lines\n";
      }

      if ($opt_xra) {
	      print OUT "set xrange [ $opt_xra :]\n";
      }
      if ($opt_xrb) {
	      print OUT "set xrange [ :$opt_xrb]\n";
      }

      if ($opt_yra) {
	      print OUT "set yrange [ $opt_yra :]\n";
      }
      if ($opt_yrb) {
	      print OUT "set yrange [ : $opt_yrb]\n";
      }

      $opt_lx && print OUT "set logscale x\n";
      $opt_ly && print OUT "set logscale y\n";

      print OUT "set data st impulses\n";
      print OUT "set ytics ( \"";

      for  (my $a = 0; $a < $qnum-1; $a+=($qnum/10)) {
	      print OUT &UnixDate($date[$a],"%Y/%m/%d %H:%M %a");
              print OUT "\" $a, \"";
      }
      print OUT &UnixDate($date[$qnum],"%Y/%m/%d %H:%M %a");
      print OUT "\" $qnum)\n";
      print OUT "set title \"$statistic{$predef[$k-1]}\"\n";

      print OUT "splot '$opt_b.data' matrix t \"\"\n"; 
      #		print OUT "plot '$opt_b.data' using :1 t \"\"\n"; 
      print OUT "\n";
}

sub trace_is_good() {
	# questa funzione mi dice se la traccia ha una data (o un'ora) che mi piace.

	$q_date = shift;

	#if ((&Date_Cmp($s_date,$q_date) <= 0) && (&Date_Cmp($q_date,$e_date) < 0)) {
	if ( $all_dates_are_good || (($s_date le $q_date) && ($q_date le $e_date))) {
		if (!$opt_o) {
			return 2;
		} else {
			print "$q_date" if $VERBOSE;
			$q_date =~ /(\d\d):(\d\d):\d\d$/;
			$q_hour = $1*60+$2;
			print " $q_hour\n" if $VERBOSE;
			if (($q_hour>=$s_hour) && ($q_hour<$e_hour)) {
				return 2;
			} else {
				return 1;
			}
		}
	} else {
		return 0;
	}	
}
