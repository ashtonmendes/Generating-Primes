
<HTML>
<HEAD>
<TITLE>Homework #7 - CSCI 531, Spring 2015</TITLE>
<LINK HREF="../../cs531bc.css" REL="stylesheet" TYPE="text/css">
<LINK REV="made" HREF="mailto:bill.cheng@acm.org"></HEAD>
<BODY BGCOLOR="#FFFFFF" LINK="#D02090" VLINK="#996600" ALINK="#990000">

<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0 WIDTH=100%>

The purpose of this assignment is to get familiar with
generating prime numbers, the
<A HREF="http://www.openssl.org/docs/crypto/bn.html">multiple
precision math library (called BIGNUM)</A> of
<A HREF="http://www.openssl.org">openssl</A>,
and modular arithematics with <TT>BIGNUM</TT>.

To use <TT>openssl</TT> on <TT>nunki.usc.edu</TT>, please
see the <A HREF="http://merlot.usc.edu/cs531-s15/openssl.html">additional notes on <TT>openssl</TT></A>.

<P>
</TD>
</TR>
<TR>Compiling:</TR>
<PRE>
    make hw7</PRE>
an executable named <B>hw7</B> is created.

<FONT COLOR="#ffffff"><A
NAME="syntax"><B>Commandline Syntax & Program
Output</B></A></FONT>
</TD>
</TR>
<TR><TD COLSPAN=3 ALIGN=LEFT>
The commandline syntax for <B>hw7</B> is as follows:

<PRE>
    <B>hw7</B> <B>primes</B> <B>-n=</B>maxval
    <B>hw7</B> <B>trialdiv</B> <B>-n=</B>number <B>-p=</B>primesfile
    <B>hw7</B> <B>millerrabin</B> <B>-n=</B>number <B>-t=</B>maxitr <B>-p=</B>primesfile
    <B>hw7</B> <B>rndsearch</B> <B>-k=</B>numbits <B>-t=</B>maxitr <B>-p=</B>primesfile <B>-r=</B>rndfile
    <B>hw7</B> <B>maurer</B> <B>-k=</B>numbits <B>-p=</B>primesfile <B>-r=</B>rndfile</PRE>
<P>

Square bracketed items are optional.
Follows the UNIX convention that <B>commandline options</B>
can come in any order.  (Note: a <B>commandline option</B> is a
commandline argument that begins with a <B>-</B> character in a
commandline syntax specification.)
If an input file is not specified, the program
reads from <TT>stdin</TT>.
Unless otherwise specified,
output of the program goes to <TT>stdout</TT> and
error messages go to <TT>stderr</TT>.
<TT>number</TT> is in decimal and all output numeric values are in decimal.

<P>

The meaning of the commands are:

<BLOCKQUOTE>
<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>primes</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            Generate all prime numbers less than or equal to
            <TT>maxval</TT>.  <TT>maxval</TT> must be between
            2 and 2<SUP><FONT SIZE=-2>24</FONT></SUP>, inclusive.
            You can use the
            <A HREF="../sieve.html">sieve of Eratosthenes</A>
            to generate the needed prime numbers.
            <P>
            Since the sieve of Eratosthenes uses an array
            of <TT>maxval</TT> integers whose values can only be
            0 or 1, please be memory
            efficient and use only <TT>maxval</TT> bits (i.e.,
            <TT>ceil(maxval/8)</TT> bytes) for the array.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>trialdiv</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            Test the primality of <TT>number</TT> using
            <I>trial division</I> by trying sequentially
            all small prime numbers from <TT>2</TT> to
            <TT>floor(sqrt(number))</TT>, inclusive.  <TT>primesfile</TT>
            conforms to the
            <A HREF="#format"><TT>primesfile</TT> format</A>
            and must be used as the source of small prime
            numbers.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>millerrabin</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            Test the primality of <TT>number</TT> using
            <A HREF="#millerrabin">
            Miller-Rabin specified here</A>
            with security parameter
            <TT>maxitr</TT>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>rndsearch</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            Generate a <TT>numbits</TT>-bit probable prime using the
            <A HREF="#rndsearch"><TT>Random-Search(numbits,maxitr)
            algorithm specified here</TT></A>
            with security parameter <TT>maxitr</TT>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>maurer</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            Generate a <TT>numbits</TT>-bit provable prime using the
            <A HREF="#maurer"><TT>Maurer(numbits)
            algorithm specified here</TT></A>.
        </TD>
    </TR>
</TABLE>
</BLOCKQUOTE>

<P>

The output for various commands are as follows.

<BLOCKQUOTE>
<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>primes</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            The output of this command is a file in the
            <A HREF="#format"><TT>primesfile</TT> format</A>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>trialdiv</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            If <TT>number</TT> passes the <I>trial division</I> test,
            your program should output the following string (please do
            <I>not</I> replace "n" with <TT>number</TT>):
  <PRE>
  n passes trial division test</PRE>
            If the <TT>maxval</TT> in <TT>primesfile</TT>
            is strictly less than <TT>floor(sqrt(number))</TT> and
            all prime numbers in <TT>primesfile</TT> are not divisors
            of <TT>number</TT>,
            your program should output the following string (please do
            <I>not</I> replace "n" with <TT>number</TT>):
  <PRE>
  n passes trial division test (not enough primes)</PRE>
            If <TT>number</TT> fails the <I>trial division</I> test
            because it was divisible by prime number <TT>m</TT>,
            your program should output the following string (please
            replace "m" with the numeric value of <TT>m</TT>):
  <PRE>
  n is composite by trial division (mod m = 0)</PRE>
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>millerrabin</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            As your program progresses through various stages of the
            <A HREF="#millerrabin">Miller-Rabin</A> algorithm,
            please print out the following information.
            <UL>
            <LI>Before you start, print the value of <TT>number</TT>.
            <LI>After step (1), print the value of <TT>n-1</TT>, <TT>s</TT>,
                and <TT>r</TT>.
            <LI>After step (2.2), print the value of <TT>a</TT> and <TT>y</TT>.
            <LI>After step (2.3.1.1), print the value of <TT>j</TT> and
                <TT>y</TT>.
            <LI>After Miller-Rabin returns, print the result.
            </UL>
            Please use the specific format illustrated here.
            Below is an
            example if no strong witness is found (i.e., Miller-Rabin
            returns "prime") and <TT>maxitr=10</TT>:
  <PRE>
  n = 1066283
    n-1 = 1066282
    s = 1
    r = 533141
    Itr 1 of 10, a = 2, y = 1066282 (which is n-1)
    Itr 2 of 10, a = 3, y = 1
    Itr 3 of 10, a = 5, y = 1066282 (which is n-1)
    Itr 4 of 10, a = 7, y = 1066282 (which is n-1)
    Itr 5 of 10, a = 11, y = 1066282 (which is n-1)
    Itr 6 of 10, a = 13, y = 1
    Itr 7 of 10, a = 17, y = 1
    Itr 8 of 10, a = 19, y = 1
    Itr 9 of 10, a = 23, y = 1066282 (which is n-1)
    Itr 10 of 10, a = 29, y = 1066282 (which is n-1)
  Miller-Rabin declares n to be a prime number</PRE>
            Please note that there is no leading space characters
            for the first and last lines and there are two space characters
            for the other lines above.
            <P>
            Below is an example if a strong witness is found
            (i.e., Miller-Rabin returns "composite"):
  <PRE>
  n = 1066281
    n-1 = 1066280
    s = 3
    r = 133285
    Itr 1 of 10, a = 2, y = 728675
      j = 1 of 2, y = 902584
      j = 2 of 2, y = 1066279
  Miller-Rabin found a strong witness 2</PRE>
            In the above example, the "2" in "j = 1 of 2" is just
            <TT>s-1</TT> (please see step 2.3.1 of
            <A HREF="#millerrabin">Miller-Rabin</A>).
            Please note that there are four leading space characters
            for the lines that begins with <TT>"j ="</TT>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>rndsearch</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            As your program progresses through various stages of the
            <A HREF="#rndsearch">Random-Seearch</A> algorithm,
            please print out the following information.
            <UL>
            <LI>At the beginning of step (1),
                print the iteration number.
            <LI>After step (1), print the value of <TT>n</TT>.
            <LI>If <TT>TrialDivision()</TT> returns "fail", print which
                small prime number is a divisor of <TT>n</TT>; otherwise,
                print a message that says that <TT>n</TT> passed trial
                division.
            <LI>In step (3), during Miller-Rabin, please print the same
                information as in Miller-Rabin, except that every line
                of output is indented by 2 space characters to the right.
            </UL>
            Please use the specific format illustrated here.  Below is an
            example if trial division failed during the first few iterations:
  <PRE>
  RANDOM-SEARCH: iteration 1
    n = 259
    n is composite by trial division (mod 7 = 0)
  RANDOM-SEARCH: iteration 2
    n = 447
    n is composite by trial division (mod 3 = 0)
  RANDOM-SEARCH: iteration 3
    n = 413
    n is composite by trial division (mod 7 = 0)
  RANDOM-SEARCH: iteration 4
    ...</PRE>
            Please note that there is no leading space characters
            for lines that starts with <TT>"RANDOM-SEARCH:"</TT> and
            there are two space characters for the other lines above.
            <P>
            Below is an example if finally Miller-Rabin returns "prime":
  <PRE>
  RANDOM-SEARCH: iteration 6
    n = 367
    n passes trial division test
      n-1 = 366
      s = 1
      r = 183
      Itr 1 of 20, a = 2, y = 1
      Itr 2 of 20, a = 3, y = 366 (which is n-1)
      Itr 3 of 20, a = 5, y = 366 (which is n-1)
      Itr 4 of 20, a = 7, y = 1
      Itr 5 of 20, a = 11, y = 366 (which is n-1)
      ...
      Itr 19 of 20, a = 67, y = 1
      Itr 20 of 20, a = 71, y = 366 (which is n-1)
    Miller-Rabin declares n to be a prime number</PRE>
            Please note that there are additional two leading space characters
            for the output from Miller-Rabin above.  You might want your
            Miller-Rabin routine to use an exact argument to be the base
            leading characters.
        </TD>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>maurer</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            As your program progresses through various stages of the
            <A HREF="#maurer">Maurer</A> algorithm,
            please print out the following information.
            <UL>
            <LI>Since <TT>Maurer(k)</TT> is a recursive routine, please
                assign level 0 to be the top level.  At each recursive
                call to <TT>Maurer()</TT>, print the level and
                <TT>k</TT>.
            <LI>If <TT>k &lt;= 20</TT>, print the value of <TT>n</TT>
                after step (1.1).
                If <TT>TrialDivision()</TT> returns "fail" in step (1.2),
                print the reason why <TT>TrialDivision()</TT> failed.
                Otherwise, print that <TT>n</TT> passed
                <TT>TrialDivision()</TT>.
            <LI>In step (4), print the value of <TT>r</TT> in percent.
                Please <A HREF="#round">round</A>
                to the nearest integer value of percent.
                If the (4.2) path is taken, also print the <B>last</B> return
                value of <TT>RndByte()</TT> when it broke out of the loop at
                (4.2.3).
            <LI>After step (5) returns, print the current level,
                <TT>k</TT>, and <TT>q</TT>.
            <LI>After step (7.1), print the iteration number in (7) and the
                value of <TT>R</TT> and <TT>n</TT>.
            <LI>If <TT>TrialDivision()</TT> returns "fail" in step (7.2),
                print the reason why <TT>TrialDivision()</TT> failed.
                Otherwise, print that <TT>n</TT> passed
                <TT>TrialDivision()</TT>.
            <LI>After step (7.2.1), print the value of <TT>a</TT>.
            <LI>After level 0 of Maurer's algorithm returns, print
                the returned prime number.
            </UL>
            Please use the specific format illustrated here.  Below is an
            example for generating a 40-bit prime number:
  <PRE>
Maurer: level 0, k=40
  step 4, r = 50%
Maurer: level 1, k=21
  step 4, r = 50%
Maurer: level 2, k=11
  step 1.1, n = 1027
    n is composite by trial division (mod 13 = 0)
  step 1.1, n = 1471
    n passes trial division test
Maurer: back to level 1, k=21, q=1471
  step 7, itr 1: R = 413, n = 1215047
    n passes trial division test
  step 7.2.1, itr 1: a = 1141677
Maurer: back to level 0, k=40, q=1215047
  step 7, itr 1: R = 202779, n = 492772031227
    n is composite by trial division (mod 7 = 0)
  step 7, itr 2: R = 142539, n = 346383168667
    n is composite by trial division (mod 148693 = 0)
  step 7, itr 3: R = 220807, n = 536581765859
    n is composite by trial division (mod 59 = 0)
  step 7, itr 4: R = 160387, n = 389755486379
    n is composite by trial division (mod 7 = 0)
  step 7, itr 5: R = 174443, n = 423912887643
    n is composite by trial division (mod 3 = 0)
  step 7, itr 6: R = 219009, n = 532212456847
    n passes trial division test
  step 7.2.1, itr 6: a = 335622790399

Maurer's Algorithm found an 40-bit prime:
  n = 532212456847</PRE>
            Please note that there is no leading space characters
            for lines that starts with <TT>"Maurer:"</TT>.
            <P>
            When Maurer's Algorithm terminates, please use
            <TT>BN_num_bits()</TT> to report the <B>actual</B> number of
            bits in the generated prime number <TT>n</TT>.
            <P>
        </TD>
    </TR>
</TABLE>
</BLOCKQUOTE>

<P>
</TD>
</TR>
<TR><TD>&nbsp;</TD></TR>

<TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
<FONT COLOR="#ffffff"><A NAME="format">
<B><TT>primesfile</TT> Format</B></A></FONT>
</TD>
</TR>

<TR><TD COLSPAN=3 ALIGN=LEFT>
If there are <TT>m</TT> primes between <TT>2</TT> and
<TT>maxval</TT>, inclusive, the output file must contain
exactly <TT>m+1</TT> words (a word is 4-bytes long and in
<A HREF="../byteorder.html">big endian</A> representation).
<P>
The first word you must output is <TT>maxval</TT>.  It is
followed by all the prime numbers, in increasing order,
between <TT>2</TT> and <TT>maxval</TT>, inclusive.
<P>
</TD>
</TR>
<TR><TD>&nbsp;</TD></TR>

<TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
<FONT COLOR="#ffffff"><A NAME="millerrabin">
<B>Miller-Rabin</B></A></FONT>
</TD>
</TR>

<TR><TD COLSPAN=3 ALIGN=LEFT>
For this assignment,
you are required to implement the Miller-Rabin test outlined here.  
It's slightly different from the lecture slides to make grading
more manageable.  Specifically,
step (2.1) below has been changed
to use a specified sequence of prime numbers from <TT>primesfile</TT>.
    <PRE>
    <B>Miller-Rabin(n,maxitr)</B>:
    1) write n-1 = 2<SUP>s</SUP>r such that r is odd
    2) for (i=1; i &lt;= maxitr; i++) {
       2.1) a = the i<SUP>th</SUP> smallest prime number
            2.1.1) if (a &gt; n-1) return("failure")
       2.2) compute y = a<SUP>r</SUP> mod n
       2.3) if (y != 1 and y != n-1) {
            2.3.1) for (j=1; j &lt;= s-1 and y != n-1; j++) {
                   2.3.1.1) y = y<SUP>2</SUP> mod n
                   2.3.1.2) if (y == 1) return("composite")
                   }
            2.3.2) if (y != n-1) return("composite")
            }
       }
    3) return("prime")</PRE>
            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
     
        <TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
                <FONT COLOR="#ffffff"><A NAME="rndsearch">
                <B>Random-Search</B></A></FONT>
            </TD>
        </TR>
      
        <TR><TD COLSPAN=3 ALIGN=LEFT>
For this assignment,
you are required to implement the Random-Search algorithm outlined here.  
It's slightly different from the lecture slides to make grading
more manageable.
    <PRE>
    <B>Random-Search(k,maxitr)</B>:
    1) n = RndOddNum(k)
    2) if (TrialDivision(n) == "fail") goto step (1)
    3) if (Miller-Rabin(n,maxitr) == "prime") return(n)
       goto step (1)</PRE>
<A NAME="rndoddnum">Let</A> <TT>x = ceil(k/8)</TT>.
<TT><B>RndOddNum(k)</B></TT> first reads <TT>x</TT> bytes
from <TT>rndfile</TT> and converts the data into a BIGNUM
representation using <TT>BN_bin2bn()</TT>
(first byte read is the most significant
byte in BIGNUM).
<TT>RndOddNum(k)</TT> then sets both bit zero
(the least significant bit) and bit <TT>k-1</TT>
to one, sets all bits from <TT>k</TT> and above to zero in
the BIGNUM, and returns the resulting BIGNUM.  Please make sure
that exactly <TT>x</TT> bytes of <TT>rndfile</TT> are consumed.
If <TT>k=5</TT>, and the byte you read is <TT>0xca</TT>
(which is <TT>1100 1010</TT> in binary), <TT>RndOddNum(k)</TT>
should return a 5-bit integer <TT>0x1b</TT> (which is
<TT>0001 1011</TT> in binary and <TT>27</TT> in decimal).

<P>

<A NAME="trialdiv"><TT><B>TrialDivision(n)</B></TT></A> tries
all prime numbers that are less than or equal to <TT>sqrt(n)</TT> in
<TT>primesfile</TT> in increasing order to see if it is a divisor of <TT>n</TT>.
It returns "fail" if a prime divisor is found.
Otherwise, it returns "pass".  (It should return "error" if there
are not enough prime numbers in <TT>primesfile</TT>.)

<P>

Finally, <A HREF="#millerrabin">Miller-Rabin(n,maxitr)</A>
is specified above.
            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
   
        <TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
                <FONT COLOR="#ffffff"><A NAME="maurer">
                <B>Maurer's Algorithm</B></A></FONT>
            </TD>
        </TR>
      
        <TR><TD COLSPAN=3 ALIGN=LEFT>
For this assignment,
you are required to implement Maurer's Algorithm outlined here.  
It's slightly different from the lecture slides to make grading
more manageable.
    <PRE>
    <B>Maurer(k)</B>:
    1) if (k &lt;= 20) do forever {
       1.1) n = RndOddNum(k)
       1.2) if (TrialDivision(n) == "pass") return n
       }
    2) c = 0.1, m = 20
    3) B = c * k^2 (not used)
    4) (generate a fraction r, the size of q relative to n)
       4.1) if (k &lt;= 2m) r = 0.5
       4.2) if (k &gt; 2m) do forever {
            4.2.1) r = RndByte() / 255.0
            4.2.2) r = 0.5 + r / 2.0
            4.2.3) if (k*(1-r) &gt; m) break;
            }
    5) (recursion) q = <B>Maurer(floor(r*k)+1)</B>
    6) num_bits_in_q = BN_num_bits(q)
       I = floor(2^(k-2) / q)
    7) do forever {
       7.1) R = RndOddNum(k+1-num_bits_in_q)
            R = (R mod I) + I + 1
            n = 2Rq+1
       7.2) if (TrialDivision(n) != "fail") {
                   num_bits_in_n = BN_num_bits(n)
            7.2.1) do {
                       a = RndOddNum(num_bits_in_n)
                   } while (a &lt;= 1 or a &gt;= n-1))
            7.2.2) b = a^{n-1} mod n
                   if (b == 1) {
                       b = a^{2R} mod n
                       d = gcd(b-1,n)
                       if (d == 1) return(n)
                   }
            }
       }</PRE>
<A HREF="#rndoddnum"><TT><B>RndOddNum()</B></TT></A> and
<A HREF="#trialdiv"><TT><B>TrialDivision()</B></TT></A> are defined above.

<P>

<TT><B>RndByte()</B></TT> simply reads exactly one byte
from <TT>rndfile</TT> and returns the data as an
octet (or <TT>unsigned char</TT>).
            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
        
        <TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
                <FONT COLOR="#ffffff"><A
                NAME="testdata"><B>Test Data &amp; Sample Output</B></A>
                    </FONT>
            </TD>
        </TR>
  
        <TR><TD COLSPAN=3 ALIGN=LEFT>
<B>primes</B>
<BLOCKQUOTE>
    Running the <TT>"hw7 primes -n=7"</TT> command should generate
    4 prime numbers, i.e., 2, 3, 5, and 7.
    The output file is provided <A HREF="primes.n7">here as
    <TT>primes.n7</TT></A>.
    <P>
    Running the <TT>"hw7 primes -n=1048576"</TT>
    (1048576 = 2<SUP><FONT SIZE=-2>20</FONT></SUP>)
    command should generate 82025 prime numbers.
    The output file is provided <A HREF="primes.n220">here as
    <TT>primes.n220</TT></A>.
</BLOCKQUOTE>

<B>trialdiv</B>
<BLOCKQUOTE>
    Running the <TT>"hw7 trialdiv -n=13 -p=primes.n220"</TT> or the
    <TT>"hw7 trialdiv -n=532212456847 -p=primes.n220"</TT> command
    should produce the <TT>"n passes trial division test"</TT> message.
    <P>
    Running the <TT>"hw7 trialdiv -n=667 -p=primes.n220"</TT>
    command should produce the
    <TT>"n is composite by trial division (mod 23 = 0)"</TT>
    message.
</BLOCKQUOTE>

<B>millerrabin</B>
<BLOCKQUOTE>
    Running the <TT>"hw7 millerrabin -n=13 -t=20 -p=primes.n220"</TT> command
    should get an error message saying that either <TT>maxitr</TT> is too large
    or that there is not enough primes in <TT>primes.n220</TT>.
    <P>
    Running the <TT>"hw7 millerrabin -n=532212456847 -t=20 -p=primes.n220"</TT>
    command result in Miller-Rabin declaring <TT>n</TT> to be a prime number.
    The output file is provided <A HREF="mrprime.k40">here as
    <TT>mrprime.k40</TT></A>.
    <P>
    Running the <TT>"hw7 millerrabin -n=667 -t=20 -p=primes.n220"</TT>
    command result in Miller-Rabin declaring <TT>n</TT> to be a composite
    number since a strong witness of 2 is found.
    <P>
    Running the <TT>"hw7 millerrabin -n=532212456849 -t=20 -p=primes.n220"</TT>
    command result in Miller-Rabin declaring <TT>n</TT> to be a composite
    number since a strong witness of 2 is found.
    The output file is provided <A HREF="mrcomp.k40">here as
    <TT>mrcomp.k40</TT></A>.
</BLOCKQUOTE>

<B>rndsearch</B>
<BLOCKQUOTE>
    A sample 2<SUP><FONT SIZE=-2>20</FONT></SUP> bytes long <TT>rndfile</TT>
    is provided here as <A HREF="rnd.220"><TT>rnd.220</TT></A>.
    <P>
    Please click on the links below to see sample output of the
    corresponding commands:
  <PRE>
  hw7 rndsearch -k=10 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.10">rs.10</A>
  hw7 rndsearch -k=11 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.11">rs.11</A>
  hw7 rndsearch -k=12 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.12">rs.12</A>
  hw7 rndsearch -k=13 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.13">rs.13</A>
  hw7 rndsearch -k=14 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.14">rs.14</A>
  hw7 rndsearch -k=128 -t=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="rs.128">rs.128</A></PRE>
</BLOCKQUOTE>

<B>maurer</B>
<BLOCKQUOTE>
    Please click on the links below to see sample output of the
    corresponding commands:
  <PRE>
  hw7 maurer -k=10 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.10">ma.10</A>
  hw7 maurer -k=24 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.24">ma.24</A>
  hw7 maurer -k=32 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.32">ma.32</A>
  hw7 maurer -k=64 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.64">ma.64</A>
  hw7 maurer -k=96 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.96">ma.96</A>
  hw7 maurer -k=128 -p=primes.n220 -r=rnd.220 &gt; <A HREF="ma.128">ma.128</A></PRE>
</BLOCKQUOTE>
</TD>
</TR>
<TR><TD>&nbsp;</TD></TR>

<TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
<FONT COLOR="#ffffff"><A
NAME="grading"><B>Grading Guidelines</B></A>
    </FONT>
</TD>
</TR>

<TR><TD COLSPAN=3 ALIGN=LEFT>

</BODY>
</HTML>

