<?PHP
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2006 Yoshi.Sakai @ Bluemoon inc.         //
//                       <http://www.bluemooninc.biz/>                       //
// ------------------------------------------------------------------------- //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
/******************************************************************************
XOOPS Header
******************************************************************************/
require('../../mainfile.php');
require(XOOPS_ROOT_PATH.'/header.php');
/******************************************************************************
$_SESSIONÊÑ¿ô¤è¤ê¥µ¡¼¥Ù¥¤¤ÎÉ¬Í×¥Ç¡¼¥¿¤òÀÚ¤ê½Ð¤·¤Þ¤¹¡£......> qid ¤òÅ¬µ¹ÊÑ¹¹¤¯¤À¤µ¤¤¡£
******************************************************************************/
// qid¤ÎÃµ¤·ÊýÎã¡ä
//<td class="even" align="left" >­¡Ì¾¾Î</td>
//<td class="odd" align="left"><input type="text" size="30" name="31" />
//ÆþÎÏ¤µ¤ì¤¿Ì¾¾Î¤ò½¦¤¤¤¿¤¤¾ì¹ç¤Ï¡¢¥¢¥ó¥±¡¼¥È¤Î¥½¡¼¥¹¤òÉ½¼¨¤µ¤»¤Æ¡¢Ê¸»úÎó¡Ö­¡Ì¾¾Î¡×¤òÃµ¤·¤½¤ÎÄ¾¸å¤Ë¤¢¤ëname="31"¤Î¿ô»úÉôÊ¬¤¬qid¤Ç¤¹¡£
//
$sids = array();									// ¥µ¡¼¥Ù¥¤ID¤Î¼ý½¸Àè¤ò³ÎÊÝ
foreach($_SESSION['bmsurvey'] as $key => $val) {	// SESSIONÊÑ¿ô¤òÅ¸³«
	if (!in_array($val['sid'], $sids))				// ¼ý½¸ºÑ¤ß¥µ¡¼¥Ù¥¤ID°Ê³°¤Ê¤é
		$sids[]=$val['sid'];						// ²óÅúºÑ¤ß¥µ¡¼¥Ù¥¤¤ÎIDÇÛÎó¤ØÄÉ²Ã
	if ($val['qid'] == 31)							// ÆþÎÏÃÍ¤ò½¦¤¤¤¿¤¤¼ÁÌä¤òqid¤Ç»ØÄê¤·......> Ç¤°Õ¤ÎqidÃÍ¤ØÊÑ¹¹¤¯¤À¤µ¤¤¡£
		$inputname = $val['val'];					// ¤½¤ÎÃÍ¤òÊÑ¿ô¤ØÂåÆþ¤¹¤ë
	if ($val['qid'] == 142)							// ¼ÁÌäÊ¬´ô¤¹¤ë Multiple Choice ¤Î qid......> Ç¤°Õ¤ÎqidÃÍ¤ØÊÑ¹¹¤¯¤À¤µ¤¤¡£
		$choiced_menu = explode("|",$val['val']);	// ÁªÂò¹àÌÜ¤ÎChoice ID (question_choice¥Æ¡¼¥Ö¥ë»²¾È)¤òÇÛÎó¤Ë³ÊÇ¼
}
/******************************************************************************
¥á¥Ë¥å¡¼Ê¸»úÎó¤ÎÄêµÁ......> cid,sid,title,url ¤òÅ¬µ¹ÊÑ¹¹¤¯¤À¤µ¤¤¡£
******************************************************************************/
// Usage :
//   'cid' : ¥Þ¥ë¥Á¥Á¥ç¥¤¥¹¤ÇÁªÂò¤µ¤ì¤ë¹àÌÜ¤ÎID¤Ç¤¹¡£Ê¬´ô¸µ¥¢¥ó¥±¡¼¥È¤Î¥½¡¼¥¹¤Ç³ºÅö²Õ½ê¤ò¸¡º÷¤¹¤ë¤«bmsurvey_question_choice¥Æ¡¼¥Ö¥ë¤è¤ê¸¡º÷¤¯¤À¤µ¤¤¡£
//           Îã¡ä<input type="checkbox" name="142[]" value="384" />¼ÁÌäÆâÍÆÊ¸»úÎó.... ¤³¤Î¾ì¹ç¤Ï¡¢qid=142 cid=384 ¤È¤¤¤¦»ö¤Ë¤Ê¤ê¤Þ¤¹¡£
//   'sid' : ¥µ¡¼¥Ù¥¤ID¤Ç¤¹¡£¥µ¡¼¥Ù¥¤´ÉÍý¤Î°ìÍ÷¤Îº¸Ã¼¤Î¥Ê¥ó¥Ð¡¼¤Ç»ØÄê¤·¤Þ¤¹¡£¥Æ¡¼¥Ö¥ë¤Ç¤Ï¡¢bmsurvey_survey¤è¤ê¸¡º÷¤Ç¤­¤Þ¤¹¡£
// 'title' : ¥µ¥Ö¥á¥Ë¥å¡¼¤ËÉ½¼¨¤¹¤ë¥¢¥ó¥±¡¼¥È¤Î¥¿¥¤¥È¥ëÊ¸»úÎó¤Ç¤¹¡£HTML¥¿¥°¤òÆþ¤ì¤ì¤ÐÁõ¾þ¤Ç¤­¤Þ¤¹¡£
//   'url' : ¥µ¡¼¥Ù¥¤¤ÎURL¤ò»ØÄê¤·¤Þ¤¹¡£
$survey_url = XOOPS_URL."/modules/bmsurvey/survey.php?name=";
$menus = array(
array('cid'=>"384", 'sid'=>"3",  'title'=>"1. ¼ÁÌä£±¤Ç£Á¤ÈÅú¤¨¤¿Êý¤Ø¤ÎÄÉ²Ã¼ÁÌä", 'url'=>$survey_url . "detail_a"),
array('cid'=>"385", 'sid'=>"4",  'title'=>"2. ¼ÁÌä£±¤Ç£Â¤ÈÅú¤¨¤¿Êý¤Ø¤ÎÄÉ²Ã¼ÁÌä", 'url'=>$survey_url . "detail_b"),
array('cid'=>"386", 'sid'=>"5",  'title'=>"3. ¼ÁÌä£±¤Ç£Ã¤ÈÅú¤¨¤¿Êý¤Ø¤ÎÄÉ²Ã¼ÁÌä", 'url'=>$survey_url . "detail_c"),
);
/******************************************************************************
°Ê²¼¡¢¼èÆÀ¤·¤¿ÊÑ¿ô¤òÍøÍÑ¤·¤Æ¥á¥Ã¥»¡¼¥¸¤ä¥á¥Ë¥å¡¼¤ÎÆ°ºî¤ò½èÍý¤·¤Þ¤¹¡£
******************************************************************************/
echo "<H2>".$inputname."¤µ¤ó¡¢¥¢¥ó¥±¡¼¥È²óÅúÍ­Æñ¤¦¤´¤¶¤¤¤Þ¤¹¡£°ú¤­Â³¤­³ºÅö¤¹¤ë¹àÌÜ¤Î¥¢¥ó¥±¡¼¥È¤Ë¤ªÅú¤¨¤¯¤À¤µ¤¤¡£</H2>";
if (in_array( $menus[ 0]['cid'], $choiced_menu))
	if (in_array( $menus[ 0]['sid'], $sids)) echo $menus[ 0]['title'] . '<FONT color="red">...²óÅúºÑ¤ß</FONT><BR />';
	else echo '<A HREF="'. $menus[ 0]['url'].'">'. $menus[ 0]['title'].'</A><BR />';
if (in_array( $menus[ 1]['cid'], $choiced_menu))
	if (in_array( $menus[ 1]['sid'], $sids)) echo $menus[ 1]['title'] . '<FONT color="red">...²óÅúºÑ¤ß</FONT><BR />';
	else echo '<A HREF="'. $menus[ 1]['url'].'">'. $menus[ 1]['title'].'</A><BR />';
if (in_array( $menus[ 2]['cid'], $choiced_menu))
	if (in_array( $menus[ 2]['sid'], $sids)) echo $menus[ 2]['title'] . '<FONT color="red">...²óÅúºÑ¤ß</FONT><BR />';
	else echo '<A HREF="'. $menus[ 2]['url'].'">'. $menus[ 2]['title'].'</A><BR />';
/******************************************************************************
XOOPS footer
******************************************************************************/
include(XOOPS_ROOT_PATH.'/footer.php');
?>
