<?php

# $Id: finish.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	$_SESSION['new_survey'] = false;
	$survey_name = "";
    $sql = "SELECT name FROM ".TABLE_SURVEY." WHERE id = '". $_SESSION['survey_id']. "'";
    if ($result = mysql_query($sql)) {
	    if (mysql_num_rows($result) > 0)
		    $survey_name = mysql_result($result, 0, 0);
		mysql_free_result($result);
	}

?>
<br>
<b><?php mb_echo('Survey Design Completed'); ?></b>
<div align="left">
	<p><?php mb_echo('You have completed this survey design.'); ?></p>

	<p><?php mb_echo('To insert this survey into your web page, copy the text below, and paste it into the HTML of your page.'); ?></p>

<blockquote><tt>
	&lt;?php $sid=<?php echo($_SESSION['survey_id']); ?>;
	include("<?php echo($ESPCONFIG['handler']); ?>");?&gt;
</tt></blockquote>

	<p><?php mb_echo('Once activated you can also access the survey directly from the following URL.'); ?></p>
<blockquote><tt>
	<?php echo($ESPCONFIG['autopub_url']."?name=".$survey_name); ?>
</tt></blockquote>

	<p><?php mb_echo('You must activate this survey before you can collect results. Once a survey is active, you may no longer make any changes to it. You may activate this survey by choosing <b>Change the Status of an Existing Survey</b> from the Management Interface.'); ?></p>
</div>

<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
<br>&nbsp;
