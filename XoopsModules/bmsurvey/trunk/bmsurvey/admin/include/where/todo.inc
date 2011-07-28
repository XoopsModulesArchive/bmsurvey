<?php

# $Id: todo.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<?php if (file_exists($ESPCONFIG['include_path']."/../../TODO")) { ?>
<table bgcolor="#ffffff" border="0"><tr><td><pre>
<?php include($ESPCONFIG['include_path']."/../../TODO"); ?>
</pre></td></tr></table>
<?php
	} else {
		echo('<p>' . mkwarn(_GT_Todo_list_not_found) . "</p>\n");
	}
?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
?>