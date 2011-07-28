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
<?php echo(_GT_The_information_on_this_tab_applies_to_the_whole_survey); ?>

<hr>
<table width="95%" border="0" cellspacing="0" cellpadding="0">
	<tr valign="top">
		<td><b><?php echo(_GT_Name); ?></b></td>
		<td>
			<?php echo mktext('name', 20, 64, $survey); ?>
			<font color="#ff0000"><em><?php echo(_GT_Required); ?></em></font><br>
			<?php echo(
			_GT_Survey_filename .' '.
			_GT_This_is_used_for_all_further_access_to_this_survey
			.' ('. _GT_no_spaces .', '. _GT_alpha_numeric_only .')'); ?></td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Owner); ?></b></td>
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
			echo(mkselect('realm',$realms, $survey) . "  <font color=\"#ff0000\"><em>". _GT_Required ."</em></font><br>\n");
			echo(_GT_User_and_Group_that_owns_this_survey); ?></td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Title); ?></b></td>
		<td>
			<?php echo mktext('title', 60, 60, $survey); ?>
			<font color="#ff0000"><em><?php echo(_GT_Required); ?></em></font><br>
			<?php echo(_GT_Title_of_this_survey); echo(' '.
			_GT_This_appears_at
			.' ('. _GT_free_form_including_spaces .')'); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Subtitle); ?></b></td>
		<td>
			<?php echo mktext('subtitle', 60, 128, $survey); ?><br>
			<?php echo( _GT_Subtitle_of_this_survey . ' '.
			_GT_Appears_below_the_title .' ('. _GT_free_form_including_spaces. ')'); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Additional_Info); ?></b></td>
		<td>
			<?php echo mktextarea('info', 5, 60, 'virtual', $survey); ?><br>
			<?php echo(_GT_Text_to_be_displayed_on_this_survey_before_any_fields); ?>
		</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Confirmation_Page); ?></b></td>
		<td>
			<?php echo mktext('thanks_page', 60, 255, $survey) .'&nbsp;'.
                    _GT_URL; ?><br>
			<?php echo(_GT_The_URL_to_which_a_user_is_redirected_after_completing_this_survey); ?>
			<br><b><?php echo(_GT_OR); ?></b><br>
			<?php echo mktext('thank_head', 30, 0, $survey) .'&nbsp;'.
                    _GT_heading_text; ?><br>
			<?php echo mktextarea('thank_body', 5, 60, 'virtual', $survey) .
                    '&nbsp;' . _GT_body_text; ?><br>
 			<?php echo(_GT_Heading_in_bold); ?>
			<?php echo(_GT_URL_if_present); ?>
			</td>
	</tr>
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
		<td><b><?php echo(_GT_Email); ?></b></td>
		<td>
            <?php echo mktext('email', 30, 0, $survey); ?><br>
			<?php echo(_GT_Sends_a_copy); ?></td>
	</tr>


	<tr>
		<td colspan="2"><hr></td></tr>
        <tr valign="top">
		<td><b><?php echo(_GT_From_Option); ?></b></td>
                <td><?php
				$send_array[0] = _MD_FROM_OPTION_0;
				$send_array[1] = _MD_FROM_OPTION_1;
				$send_array[2] = _MD_FROM_OPTION_2;
			echo(mkselect('from_option',$send_array, $survey) . '<br>' .
            		_MD_FROM_OPTION); ?></td>
        </tr>
		<tr><td colspan="2"><hr></td></tr>
		<tr><td><b><?php echo(_GT_Default_Response); ?></b></td>
                <td><?php
			echo(mktext('response_id',10,0, $survey) . '<br>' .
            		_MD_FROM_DEFRES); ?></td>
        </tr>


	<tr><td colspan="2"><hr></td></tr>
        <tr valign="top">
		<td><b><?php echo(_GT_Theme); ?></b></td>
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

			echo( mkselect('theme',$themes_array, $survey) . '<br>' .
            		_GT_Select_a_theme ); ?></td>
        </tr>
<!--
	<tr><td colspan="2"><hr></td></tr>
	<tr valign="top">
        <td><b><?php echo(_GT_Options); ?></b></td>
		<td><?php echo(mkcheckbox('options','resume') . '&nbsp;'); ?>
	</tr>
	<tr valign="top">
		<td>&nbsp;</td>
		<td><?php echo(mkcheckbox('options','navigate') . '&nbsp;' .
				_GT_Allow_to_save); ?></td>
-->                
</table>
