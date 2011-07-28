<?php

# $Id: general.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>
	unset($sid);
	$sid = $_SESSION['survey_id'];

	// load current values from DB if $sid exists (in session)
	$survey = array();
	if(!empty($sid) && $updated) {
		$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
		$result = mysql_query($sql);
		$survey = mysql_fetch_array($result,MYSQL_ASSOC);
		mysql_free_result($result);
	} else {
		$fields = array('name','realm','title','subtitle','email','theme','thanks_page','thank_head','thank_body','info','public');
	   	foreach ($fields as $f) {
            if(!empty($_POST[$f]))
	    	    $survey[$f] = _stripslashes($_POST[$f]);
            else
                $survey[$f] = '';
        }
	}
?>
<?php echo(mb_('The information on this tab applies to the whole survey. Fill out this page then go to the <b>Fields</b> tab to edit individual fields.')); ?>

<hr>
<table width="95%" border="0" cellspacing="0" cellpadding="0">
	<tr valign="top">
		<td><b><?php mb_echo('Name'); ?></b></td>
		<td>
			<?php echo mktext('name', 20, 64, $survey); ?>
			<font color="#ff0000"><em><?php mb_echo('Required'); ?></em></font><br>
			<?php echo(
			mb_('Survey filename.') .' '.
			mb_('This is used for all further access to this survey.')
			.' ('. mb_('no spaces') .', '. mb_('alpha-numeric only') .')'); ?></td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Owner'); ?></b></td>
		<td><?php
			$realms = array();
			if($_SESSION['acl']['superuser'] == 'Y') {
				/*
				$sql = "SELECT name FROM ".TABLE_REALM.";";
				$rr = mysql_query($sql);
				while(list($r) = mysql_fetch_row($rr))
					$realms[$r] = $r;
				mysql_free_result($rr);
				*/
				//$realms = $member_handler->getGroupsByUser( $xoopsUser->getVar("uid") ) ;
				$realms = $member_handler->getGroupList(); 
			} else {
				foreach($_SESSION['acl']['pdesign'] as $r)
					$realms[$r] = $r;
			}
			echo(htmlspecialchars($_SESSION['acl']['username']) . "&nbsp;/&nbsp;");
			echo(mkselect('realm',$realms, $survey) . "  <font color=\"#ff0000\"><em>". mb_('Required') ."</em></font><br>\n");
			mb_echo('User and Group that owns this survey.'); ?></td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Title'); ?></b></td>
		<td>
			<?php echo mktext('title', 60, 60, $survey); ?>
			<font color="#ff0000"><em><?php mb_echo('Required'); ?></em></font><br>
			<?php mb_echo('Title of this survey.'); echo(' '.
			mb_('This appears at the top of every page of this survey.')
			.' ('. mb_('free-form, including spaces') .')'); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Subtitle'); ?></b></td>
		<td>
			<?php echo mktext('subtitle', 60, 128, $survey); ?><br>
			<?php mb_echo('Subtitle of this survey.'); echo(' '.
			mb_('Appears below the title.') .' ('. mb_('free-form, including spaces'). ')'); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Additional Info'); ?></b></td>
		<td>
			<?php echo mktextarea('info', 5, 60, 'virtual', $survey); ?><br>
			<?php mb_echo('Text to be displayed on this survey before any fields. (i.e. instructions, background info, etc.)'); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Confirmation Page'); ?></b></td>
		<td>
			<?php echo mktext('thanks_page', 60, 255, $survey) .'&nbsp;'.
                    mb_('(URL)'); ?><br>
			<?php mb_echo('The URL to which a user is redirected after completing this survey.'); ?>
			<br><b><?php mb_echo('OR'); ?></b><br>
			<?php echo mktext('thank_head', 30, 0, $survey) .'&nbsp;'.
                    mb_('(heading text)'); ?><br>
			<?php echo mktextarea('thank_body', 5, 60, 'virtual', $survey) .
                    '&nbsp;' . mb_('(body text)'); ?><br>
 			<?php mb_echo('Heading (in bold) and body text for the &quot;Confirmation&quot; page displayed after a user completes this survey.'); ?>
			<?php mb_echo('(URL, if present, takes precedent over confirmation text.)'); ?>
			</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php mb_echo('Email'); ?></b></td>
		<td>
            <?php echo mktext('email', 30, 0, $survey); ?><br>
			<?php mb_echo('Sends a copy of each submission to address (or leave blank for no email backup).'); ?></td>
	</tr>


	<tr>
		<td colspan="2"><hr></td></tr>
        <tr valign="top">
		<td><b><?php mb_echo('From Option'); ?></b></td>
                <td><?php
				$send_array[0] = _MD_FROM_OPTION_0;
				$send_array[1] = _MD_FROM_OPTION_1;
				$send_array[2] = _MD_FROM_OPTION_2;
			echo(mkselect('from_option',$send_array, $survey) . '<br>' .
            		mb_(_MD_FROM_OPTION)); ?></td>
        </tr>
		<tr><td colspan="2"><hr></td></tr>
		<tr><td><b><?php mb_echo('Default Response'); ?></b></td>
                <td><?php
			echo(mktext('response_id',10,0, $survey) . '<br>' .
            		mb_(_MD_FROM_DEFRES)); ?></td>
        </tr>


	<tr><td colspan="2"><hr></td></tr>
        <tr valign="top">
		<td><b><?php mb_echo('Theme'); ?></b></td>
                <td><?php
			$themes_array = array();
			$dir = dir($ESPCONFIG['css_path']);
				$dir->rewind();
				while ($file=$dir->read()) {
					if (stristr($file,".css")) {
						$pos = strrpos($file, ".");
						$name = substr($file, 0,$pos);
						$themes_array[$file] = $name;
					}
				}
				$dir->close();

			echo(mkselect('theme',$themes_array, $survey) . '<br>' .
            		mb_('Select a theme (css) to use with this survey.')); ?></td>
        </tr>
<!--
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
        <td><b><?php mb_echo('Options'); ?></b></td>
		<td><?php echo(mkcheckbox('options','resume') . '&nbsp;'); ?>
	</tr>
	<tr valign="top">
		<td>&nbsp;</td>
		<td><?php echo(mkcheckbox('options','navigate') . '&nbsp;' .
				mb_('Allow survey respondents to go forward and back between survey sections.')); ?></td>
-->                
</table>
