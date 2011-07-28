<?php
// Max upload file size
// If you want set upper 2M, You must change php.ini
//   memory_limit = 8M (default)
//   post_max_size = 8M (default)
//   upload_max_filesize =2M (default)
$maxfilesize = 2000000;			// default 2MB

if(
	!defined('XOOPS_ROOT_PATH') ||
	!defined('XOOPS_CACHE_PATH') ||
	!is_dir(XOOPS_CACHE_PATH)
){
	exit();
}
if(!empty($xoopsConfig)){
	if(file_exists(XOOPS_ROOT_PATH.'/modules/bmsurvey/language/'.$xoopsConfig['language'].'/main.php')){
		require_once XOOPS_ROOT_PATH.'/modules/bmsurvey/language/'.$xoopsConfig['language'].'/main.php';
	}
}
bmsurvey_init();

function bmsurvey_init(){
	global $xoopsDB;
	define('TABLE_REALM', $xoopsDB->prefix("bmsurvey_realm"));
	define('TABLE_RESPONDENT', $xoopsDB->prefix("bmsurvey_respondent"));
	define('TABLE_DESIGNER', $xoopsDB->prefix("bmsurvey_designer"));
	define('TABLE_SURVEY', $xoopsDB->prefix("bmsurvey_survey" ));
	define('TABLE_QUESTION_TYPE', $xoopsDB->prefix("bmsurvey_question_type" ));
	define('TABLE_QUESTION', $xoopsDB->prefix("bmsurvey_question" ));
	define('TABLE_QUESTION_CHOICE', $xoopsDB->prefix("bmsurvey_question_choice" ));
	define('TABLE_ACCESS', $xoopsDB->prefix("bmsurvey_access" ));
	define('TABLE_RESPONSE', $xoopsDB->prefix("bmsurvey_response" ));
	define('TABLE_RESPONSE_BOOL', $xoopsDB->prefix("bmsurvey_response_bool" ));
	define('TABLE_RESPONSE_SINGLE', $xoopsDB->prefix("bmsurvey_response_single" ));
	define('TABLE_RESPONSE_MULTIPLE', $xoopsDB->prefix("bmsurvey_response_multiple" ));
	define('TABLE_RESPONSE_RANK', $xoopsDB->prefix("bmsurvey_response_rank" ));
	define('TABLE_RESPONSE_TEXT', $xoopsDB->prefix("bmsurvey_response_text" ));
	define('TABLE_RESPONSE_OTHER', $xoopsDB->prefix("bmsurvey_response_other" ));
	define('TABLE_RESPONSE_DATE', $xoopsDB->prefix("bmsurvey_response_date" ));

	define('TABLE_', $xoopsDB->prefix("bmsurvey_" ));

}
?>
