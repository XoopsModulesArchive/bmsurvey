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
<b><?php echo(_GT_Survey_Design_Completed); ?></b>
<div align="left">
	<p><?php echo(_GT_You_have_completed_this_survey_design); ?></p>

	<p><?php echo(_GT_To_insert_this_survey_into_your_web_page); ?></p>

<blockquote><tt>
	&lt;?php $sid=<?php echo($_SESSION['survey_id']); ?>;
	include("<?php echo($ESPCONFIG['handler']); ?>");?&gt;
</tt></blockquote>

	<p><?php echo(_GT_Once_activated_you_can_also_access_the_survey_directly_from_the_following_URL); ?></p>
<blockquote><tt>
	<?php echo($ESPCONFIG['autopub_url']."?name=".$survey_name); ?>
</tt></blockquote>

	<p><?php echo(_GT_You_must_activate_this_survey); ?></p>
</div>

<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<br>&nbsp;
