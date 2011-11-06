<?php
// $Id$

//
// URL and Path
//
$SurveyCNF['root'] = XOOPS_URL."/modules/bmsurvey/";			// mod root url
$SurveyCNF['admin'] = $SurveyCNF['root']."admin/";			// mod admin url
$SurveyCNF['path'] = XOOPS_ROOT_PATH."/modules/bmsurvey/";	// mod root path
//
// Attached file section
//
$SurveyCNF['gd_ver'] = 2;							// PHP GD Version (0:No, 1:Ver 1, 2:Ver 2)
$SurveyCNF['uploads'] = XOOPS_ROOT_PATH.'/uploads/bmsurvey/';	// Upload folder. You should set more secure folder (ex.'c:/upload/').
$SurveyCNF['img_dir'] = "/uploads/bmsurvey/image/";			// Attach and direct image file folder. Work with XOOPS_ROOT_PATH,XOOPS_URL
$SurveyCNF['thumb_dir'] = "/uploads/bmsurvey/thumbs/";	// Thumbnail folder. Work with XOOPS_ROOT_PATH,XOOPS_URL
$SurveyCNF['w'] = 140;								// Thumbnail width pixsel 
$SurveyCNF['h'] = 160;								// Thumbnail height pixsel 
$SurveyCNF['img_ext'] = "gif|jpe?g|png|bmp|swf|3gp|avi|mov|ra?m|mpe?g|wmv";	// rename method for multimedia
$SurveyCNF['thumb_ext'] = ".+\.jpe?g$|.+\.png$|.+\.gif$";	// Thumb image target file extentions
$SurveyCNF['subtype'] = "gif|jpe?g|png|bmp|zip|lzh|rar|pdf|excel|powerpoint|octet-stream|x-pmd|x-mld|x-mid|x-smd|x-smaf|x-mpeg";	// Acceptable MIME Content-Type
$SurveyCNF['imgtype'] = "gif|jpe?g|png|bmp|x-pmd|x-mld|x-mid|x-smd|x-smaf|x-mpeg";	// Acceptable MIME for images
$SurveyCNF['embedtype'] = "video|audio|x-shockwave-flash|3gpp";	// embedding EMBED MIME Content-Type
$SurveyCNF['viri'] = "cgi|php|jsp|pl|htm";			// reject file extentions
$SurveyCNF['maxbyte'] = "2000000";					// Max attach file byte size (2M)
//
// Mail Ctrl section
//
$limit_min = 1;							// Time intreval about Auto POP ( minutes )
$JUST_POPED = "Just Poped!";			// POP execute message for URL pop
$log_dir = './log/';					// Log folder
$log = $log_dir.'maillog.cgi';			// log file ( Change the 'maillog' strings for sequrity ¡Ë
$denylog = $log_dir.'deny.cgi';			// Deny log file ( Change the 'deny' strings for sequrity ¡Ë
$denylog_save = 1;				// Save the deny log (0:No, 1:Yes)
$head_save = 0;					// Save the header infomation (0:No, 1:Yes)
$head_prefix = 'head_';			// Mail header prefix ( Change the 'head' strings for sequrity ¡Ë
$maxline = 500;							// Max saving log numbers
$adminUid = 1;					// Notice to admin for request new blog. ( null = not notify )
$nosubject = "No Title";				// Title for no subject
//
// Deny section
//
$imgonly = 0;							// Doesn't save if w/o attach file (Yes=1 No=0¡Ë
$SurveyCNF['post_limit']=10000;			// Max post charactor for body words from mail
$SurveyCNF['text_limit']=1024;			// Max charactor for recently blog, 0 = Subject only (Work with 'blockview'=1)
// Deny POP address ( w/o log )
$deny = array('163.com','bigfoot.com','boss.com');

// Deny Mailer ( w/o log )
$deny_mailer = '';	'/(Mail\s*Magic|Easy\s*DM|Friend\s*Mailer|Extra\s*Japan|The\s*Bat|IM2001)/i';

// Deny title ( w/o log )
$deny_title = '';	'/((Ì¤|Ëö)\s?¾µ\s?(Âú|Ç§)\s?¹­\s?¹ð)|Áê¸ß¥ê¥ó¥¯/i';

// Deny charctorset (w/o log )
$deny_lang = '';	'/big5|euc-kr|gb2312|iso-2022-kr|ks_c_5601-1987/i';

// Cut the '_' over 25chars ( for Ad section)
$del_ereg = "[_]{25,}";

// Delete strings from body
$word[] = "http://auction.msn.co.jp/";
$word[] = "Do You Yahoo!?";
$word[] = "Yahoo! BB is Broadband by Yahoo!";
$word[] = "http://bb.yahoo.co.jp/";

if (isset($GLOBALS)) {
    $GLOBALS['SurveyCNF'] = $SurveyCNF;
} else {
    global $SurveyCNF;
}
?>
