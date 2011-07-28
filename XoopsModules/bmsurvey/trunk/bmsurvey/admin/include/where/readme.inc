<?php

# $Id: readme.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<?php if (file_exists(ESP_BASE . '/README')) { ?>
<table bgcolor="#ffffff" border="0"><tr><td><pre>
<?php include(ESP_BASE . '/README'); ?>
</pre></td></tr></table>
<?php
	} else {
		echo('<p>' . mkwarn(_GT_README_not_found) . "</p>\n");
	}
?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>