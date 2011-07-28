<?php

# $Id: logout.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>
// <jflemer@acm.rpi.edu>

	manage_logout();
?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr>
		<td>Logged out.<br>
			<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Log_back_in . "</a>\n"); ?>
		</td>
	</tr>
</table>
&nbsp;