<?php
include '../../../mainfile.php';

include '../../../include/cp_functions.php';

	xoops_cp_header();

?>

<div><p align="center" ><b>FREQUENTLY ASKED QUESTIONS (FAQs)</b></p>
<ul>
<h1>Installation</h1>
    <ul>
        <li><a href="#i_permissions"><b>What are the correct permission settings for phpESP files?</b></a></li>
        <li><a href="#i_globals"><b>Does register globals need to be on?</b></a></li>
        <li><a href="#i_blank"><b>I'm running Windows and keep getting a blank screen - Help!</b></a></li>
        <li><a href="#i_passwd"><b>Where do I set the MySQL login passwords?</b></a></li>
        <li><a href="#i_otherdb"><b>Does phpESP support databases other than MySQL?</b></a></li>
        <li><a href="#i_language"><b>I'm having trouble changing the default language, what should I do?</b></a></li>
    </ul>
</li>
<h1>Survey Management</h1>
    <ul>
        <li><a href="#m_delete"><b>Can I delete old surveys?</b></a></li>
        <li><a href="#m_one_question"><b>How can I display only 1 question per page?</b></a></li> 
        <li><a href="#m_rank"><b>Can I change the number of options on a rank question?</b></a></li>
        <li><a href="#m_multiple"><b>Can I prevent multiple votes (ballot stuffing)?</b></a></li>
        <li><a href="#m_login"><b>Can I limit the number of times someone takes a survey if they have to login?</b></a></li>
        <li><a href="#m_part"><b>Can a respondent complete part of a survey, log out, then log back in later to finish?</b></a></li>
    </ul>
</li>
<h1>Survey Deployment</h1>
    <ul>
        <li><a href="#d_handler"><b>What are handler.php and handler-prefix.php, and how do I use them?</b></a></li>
    </ul>
</li>
<h1>General Questions</h1>
    <ul>
        <li><a href="#g_large"><b>Can phpESP support large numbers of respondents?</b></a></li>
    </ul>
</li>
</ul>
<hr>
<ul>
<h1>Installation</h1>
<p><a name="i_permissions"><b>What are the correct permission settings for phpESP files?</b></a>
<p class="indented">In a Unix environment, phpESP files should be set as 644 to be readable by Apache. For tighter security, phpESP.ini.php should be set 640, owner root and it's group set to the Apache process's group.</p>
<a href="#top">Top</a>
<p><a name="i_globals"><b>Does register globals need to be on? </b></a>
<p class="indented">No.</p>
<a href="#top">Top</a> 
<p><a name="i_blank"><b>I'm running Windows and keep getting a blank screen - Help! </b></a>
<p class="indented">If is is not a path problem in phpESP.ini.php, then copy gnu_gettext.dll (or libintl-1.dll) and 
php_gettext.dll (both usually found in the PHP install directory) into the system directory of windows.  For
Win95/98 the system directory is something like <tt>c:\windows\system</tt> and on WinNT/2000/XP the system 
directory is something like <tt>c:\windows\system32</tt>.<br>
<br>After you have copied these dll's to the proper place, edit your php.ini file and comment out the 
line "extension=php_gettext.dll". <br>
<br>Then stop and restart the web server.</p>
<a href="#top">Top</a> 
<p><a name="i_passwd"><b>Where do I set the MySQL login password?</b></a>
<p class="indented">No login Necessary. This module is already login by XOOPS core.</p>
<a href="#top">Top</a> 
<p><a name="i_otherdb"><b>Does phpESP support databases other than MySQL?</b></a>
<p class="indented">Not currently.  We would welcome help adapting phpESP to use
the PEAR database classes if you have the time.</p>
<a href="#top">Top</a> 
<p><a name="i_language"><b>I'm having trouble changing the default language, what should I do?</b></a>
<p class="indented">Check the results of the phpESP system test page, then see the TRANSLATIONS file.</p>
<a href="#top">Top</a>
</li>
<hr>
<h1>Survey Managment</h1>
<p><a name="m_delete"><b>Can I delete old surveys? </b></a>
<p class="indented">Yes, there is an unlisted function http://......./phpESP/admin/manage.php?where=purge<br>
Be <em>very</em> careful when using it, you cannot recover from it.</p>
<a href="#top">Top</a> 
<p><a name="m_one_question"><b>How can I display only 1 question per page? </b></a>
<p class="indented">You can put a section break between each question to emulate this behavior.</p>
<a href="#top">Top</a> 
<p><a name="m_rank"><b>Can I change the number of options on a rank question? </b></a>
<p class="indented">Yes, change the length field.</p>
<a href="#top">Top</a> 
<p><a name="m_multiple"><b>Can I prevent multiple votes (ballot stuffing)? </b></a>
<p class="indented">Well, if you give each respondent a username and password and have them login, you can limit 
multiple voting.  If you don't force respondents to log in, it becomes more difficult (e.g., you could 
analyze the data by IP.)</p>
<a href="#top">Top</a> 
<p><a name="m_login"><b>Can I limit the number of times someone takes a survey if they have to login? </b></a>
<p class="indented">Yes, set max login to the number of times you want to allow someone to take the survey.
This may be done from the <em>Survey Access</em> page.</p>
<a href="#top">Top</a> 
<p><a name="m_part"><b>Can a respondent complete part of a survey, log out, then log back in later to finish? </b></a>
<p class="indented">Yes, see the <em>Save/Restore</em> option on the <em>Survey Access</em> page.</p>
<a href="#top">Top</a> 
</li>
<hr>
<h1>Survey Deployment</h1>
<p><a name="d_handler"><b>What are handler.php and handler-prefix.php, and how do I use them?</b></a>
<p class="indented">They are used to embed a survey in another block of html/php code.  For private surveys, 
handler-prefix.php should be included before handler.php.  Look at the source of handler-prefix.php for 
an example.</p>
<a href="#top">Top</a>
</li>
<hr>
<h1>General Questions</h1>
<p><a name="g_large"><b>Can phpESP support large numbers of respondents?</b></a>
<p class="indented">Yes, large surveys (100 questions) and large respondent groups (4,000+) can be 
handled without a problem.  Throughput issues are generally a function of the speed of the web server, 
the speed of the MySQL server, and available network bandwidth.</p>
<a href="#top">Top</a> 
</li>
</ul>
<!-- faq  end -->
<?php
	xoops_cp_footer();
?>
