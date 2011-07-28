<?php

# $Id: funcs.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

    if (!defined('STATUS_ACTIVE')) {
        define('STATUS_ACTIVE',  0x01);
        define('STATUS_DONE',    0x02);
        define('STATUS_DELETED', 0x04);
        define('STATUS_TEST',    0x08);
    }

    if(get_magic_quotes_gpc()) {
        function _addslashes($a)    { return($a); }
        function _stripslashes($a)  { return(stripslashes($a)); }
    } else {
        function _addslashes($a)    { return(addslashes($a)); }
        function _stripslashes($a)  { return($a); }
    }
    set_magic_quotes_runtime(0);

    function esp_where($where = null)
    {
        $cfg =& $GLOBALS['ESPCONFIG'];
        if(empty($where))
            $where = 'index';
        $where = strtolower(ereg_replace(' +','_',$where));
        if(!ereg('^[A-Za-z0-9_]+$',$where))    // Valid chars are [A-Za-z0-9_]
            $where = 'index';
        if(!file_exists($cfg['include_path'].'/where/'.$where.$cfg['extension']))
            $where = 'index';
        if(!file_exists($cfg['include_path'].'/where/'.$where.$cfg['extension'])) {
            echo('<b>'. _GT_Unable_to_open_include_file. '</b>');
            exit;
        }
        return($cfg['include_path'].'/where/'.$where.$cfg['extension']);
    }

    
    // redirect to thank you page for survey ID 'sid'
    // exits PHP!
    function goto_thankyou($sid,$referer) {
        $sql = "SELECT thanks_page,thank_head,thank_body FROM ".TABLE_SURVEY." WHERE id='${sid}'";
        $result = mysql_query($sql);
        list($thank_url,$thank_head,$thank_body) = mysql_fetch_row($result);
        mysql_free_result($result);
        if(!empty($thank_url)) {
            if(!headers_sent()) {
                header("Location: $thank_url");
                exit;
            }
?>
<script language="JavaScript">
<!--
window.location="<?php echo($thank_url); ?>"
//-->
</script>
<noscript>
<h1 class="thankhead">noscript1</h1>
<h2 class="thankhead">Thank You for completing this survey.</h2>
<blockquote class="thankbody">Please click
<a class="returnlink" href="<?php echo($thank_url); ?>">here</a>
to continue.</blockquote>
</noscript>
<?php
            exit;
        }
        if(empty($thank_body) && empty($thank_head)) {
            $thank_head = _MD_BMSURVEY_THANKS_ENTRY;
        }
        return $thank_head."<p>".$thank_body;
    }

    function goto_saved($url) {
?>
<blockquote class="thankbody">
<?php echo
_GT_Your_progress_has_been_saved ?></blockquote>
<a class="returnlink" href="<?php echo $url; ?>"><?php echo _GT_Resume_survey ?></a>
<?php
        return;
    }

    require_once($ESPCONFIG['include_path']."/lib/espcross".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/espauth".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/esphtml.forms".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/esphtml.results".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/espmerge".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/espresponse".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/lib/espsql".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_copy".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_merge".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_purge".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_render".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_report".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_results".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_update".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/survey_export_csv".$ESPCONFIG['extension']);
    require_once($ESPCONFIG['include_path']."/function/account_upload".$ESPCONFIG['extension']);

?>
