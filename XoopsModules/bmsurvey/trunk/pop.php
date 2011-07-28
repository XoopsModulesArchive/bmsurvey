<?php
// $Id: pop.php,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2005 Yoshi.Sakai @ Bluemoon inc.         //
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
//
// Base source : pop.php http://php.s3.to ToR Last Modify 03/07/24 v2.61
//
/*-----------------*/
include_once('../../mainfile.php');
include_once('pop.ini.php');
include_once('thumb.php');
include_once('./class/bmsurveyUtils.php');
/*-----------------*/
include_once('./admin/phpESP.ini.php');
include_once('./admin/include/lib/espsql.inc');
include_once('./admin/include/lib/espresponse.inc');

$debug = 0;	// When you debugging this source set to 1, 0 is off.

//survey_init();
if ( !(isset($host) && $host) ) { 
	include('pop.ini.php');
}
$img_mode = false;
if (isset($_GET['img'])) {
	if ($_GET['img']) $img_mode = true;
}

// mb_関数が使えない場合http://www.spencernetwork.org/にて漢字コード変換(簡易版)を入手して下さい
//if (file_exists("jcode-LE.php")) require_once("./jcode-LE.php");

$host = bmsurveyUtils::getXoopsModuleConfig('MAILSERVER');
$user = bmsurveyUtils::getXoopsModuleConfig('MAILUSER');
$pass = bmsurveyUtils::getXoopsModuleConfig('MAILPWD');
$mail = bmsurveyUtils::getXoopsModuleConfig('MAILADDR');

if (!$host || !$user || !$pass || !$mail){
	echo "No Preferences. Set from admin.";
	return false;
}
$admin = $_GET['admin'];

// Connect start!!!
//$sock = fsockopen($host, 110, $err, $errno, 10) or error_output("Can't connect to POP Server.");
$sock = fsockopen($host, 110, $err, $errno, 30);

if (!$sock) {
	echo 'POP host='.$host;
	echo "$errno ($err)</ br>\n";
	error_output("Can't connect to POP Server.");
}
$buf = fgets($sock, 512);
if(substr($buf, 0, 3) != '+OK'){
	error_output($buf);
}
$buf = _sendcmd("USER $user");
$buf = _sendcmd("PASS $pass");
$data = _sendcmd("STAT");//STAT -件数とサイズ取得 +OK 8 1234
sscanf($data, '+OK %d %d', $num, $size);
if ($num == "0") {
	$buf = _sendcmd("QUIT"); // Quit
	fclose($sock);
	// update a log file timestamp
	@touch($log);
	
	if (!$img_mode){
		if ($admin)
			redirect_header($SurveyCNF['admin'],2,$JUST_POPED.'(no email found)');
		else
			redirect_header($SurveyCNF['root'],2,$JUST_POPED.'(no email found)');
		//header("Location: $jump");
	} else {
		// call a img tag.
		header("Content-Type: image/gif");
		readfile('spacer.gif');
	}
	exit;
}
// 件数分
for($i=1;$i<=$num;$i++) {
	$line = _sendcmd("RETR $i");//RETR n -n番目のメッセージ取得（ヘッダ含
	$dat[$i] = "";
	while (!ereg("^\.\r\n",$line)) {//EOFの.まで読む
		$line = fgets($sock,512);
		$dat[$i].= $line;
	}
	$data = _sendcmd("DELE $i");//DELE n n番目のメッセージ削除
}
$buf = _sendcmd("QUIT"); //バイバイ
fclose($sock);

$lines = array();
$lines = @file($log);
$lines = preg_replace("/\x0D\x0A|\x0D|\x0A/","\n",$lines);

for($j=1;$j<=$num;$j++) {
	$write = true;
	$filename = $subject = $from = $content = $atta = $part = $attach = $notice ="";
	list($head, $body) = mime_split($dat[$j]);
	// To:ヘッダ確認
	if (preg_match("/(?:^|\n|\r)To:[ \t]*([^\r\n]+)/i", $head, $treg)){
		$toreg = "/".quotemeta($mail)."/";
		if (!preg_match($toreg,$treg[1])) $write = false; //投稿アドレス以外
	} else {
		// To: ヘッダがない
		$write = false; $notice ="No 'To:' Header";
	}
	// メーラーのチェック
	if ($write && (eregi("(\nX-Mailer|\nX-Mail-Agent):[ \t]*([^\r\n]+)", $head, $mreg))) {
		if ($deny_mailer){
			if (preg_match($deny_mailer,$mreg[2])){ $write = false; $notice=$deny_mailer;}
		}
	}
	// キャラクターセットのチェック
	if ($write && (eregi("charset[\s]*=[\s]*([^\r\n]+)", $head, $mreg))) {
		if ($deny_lang){
			if (preg_match($deny_lang,$mreg[1])){ $write = false; $notice=$deny_lang;}
		}
	}
	// 日付の抽出
	eregi("\nDate:[ \t]*([^\r\n]+)", $head, $datereg);
	//$now = strtotime($datereg[1]);
	//print("datereg".$datereg[1]);
	//print("now".$now);
	//if ($now == -1) $now = time();
	$now = make_timestamp_for_post($datereg);
	// サブジェクトの抽出
	if (preg_match("/\nSubject:[ \t]*(.+?)(\n[\w-_]+:|$)/is", $head, $subreg)) {
		// 改行文字削除
		$subject = str_replace(array("\r","\n"),"",$subreg[1]);
		// エンコード文字間の空白を削除
		$subject = preg_replace("/\?=[\s]+?=\?/","?==?",$subject);
		// 文字間の空白も削除
		$subject = preg_replace("'([\s])'","",$subject);
		
		while (eregi("(.*)=\?iso-[^\?]+\?B\?([^\?]+)\?=(.*)",$subject,$regs)) {//MIME B
			$subject = $regs[1].base64_decode($regs[2]).$regs[3];
		}
		while (eregi("(.*)=\?iso-[^\?]+\?Q\?([^\?]+)\?=(.*)",$subject,$regs)) {//MIME Q
			$subject = $regs[1].quoted_printable_decode($regs[2]).$regs[3];
		}
		//$subject = htmlspecialchars(convert($subject));
		//$subject = htmlspecialchars(JcodeConvert($subject,0,1));
		// 未承諾広告カット
		if ($write && $deny_title){
			if (preg_match($deny_title,$subject)){ $write = false; $notice=$deny_title;}
		}
		// Get Type,Command from Subject.
		$freg = split(",",$subject);
		$keystr = $freg[0];
		if ( !$keystr ){
			$write = false; $notice="Command error at subject. [$subject]";
		}
		$cmdstr = $freg[1];
	}
	// 送信者アドレスの抽出
	if (eregi("\nFrom:[ \t]*([^\r\n]+)", $head, $freg)) {
		$from = addr_search($freg[1]);
	} elseif (eregi("\nReply-To:[ \t]*([^\r\n]+)", $head, $freg)) {
		$from = addr_search($freg[1]);
	} elseif (eregi("\nReturn-Path:[ \t]*([^\r\n]+)", $head, $freg)) {
		$from = addr_search($freg[1]);
	}

	// Check deny address
	if ($write){
		for ($f=0; $f<count($deny); $f++)
			if (eregi($deny[$f], $from)){ $write = false; $notice=$deny[$f];}
	}
	// if multipart then devide boundary 
	if (eregi("\nContent-type:.*multipart/",$head)) {
		eregi('boundary="([^"]+)"', $head, $boureg);
		$body = str_replace($boureg[1], urlencode($boureg[1]), $body);
		$part = split("\r\n--".urlencode($boureg[1])."-?-?",$body);
		if (eregi('boundary="([^"]+)"', $body, $boureg2)) {//multipart/altanative
			$body = str_replace($boureg2[1], urlencode($boureg2[1]), $body);
			$body = eregi_replace("\r\n--".urlencode($boureg[1])."-?-?\r\n","",$body);
			$part = split("\r\n--".urlencode($boureg2[1])."-?-?",$body);
		}
	} else {
		$part[0] = $dat[$j];// ordinary text mail
	}
	if ($debug){
		echo "from=$from<br>";
		print_r($part);
		print("write=".$write."<br>");
	}
	$addimg = $addfile = '';
	foreach ($part as $multi) {
		list($m_head, $m_body) = mime_split($multi);
		$m_body = ereg_replace("\r\n\.\r\n$", "", $m_body);
		// キャラクターセットのチェック
		if ($write && (eregi("charset[\s]*=[\s]*([^\r\n]+)", $m_head, $mreg))) {
			if ($deny_lang){
				if (preg_match($deny_lang,$mreg[1])){ $write = false; $notice=$deny_lang;}
			}
		}
		if (!eregi("\nContent-type: *([^;\n]+)", $m_head, $type)) continue;
		list($main, $sub) = explode("/", $type[1]);
		// 本文をデコード
		if (strtolower($main) == "text") {
			if (eregi("\nContent-Transfer-Encoding:.*base64", $m_head))
				$m_body = base64_decode($m_body);
			if (eregi("\nContent-Transfer-Encoding:.*quoted-printable", $m_head))
				$m_body = quoted_printable_decode($m_body);
			$content = trim(convert($m_body));
			//$content = JcodeConvert($m_body,0,1);
			if ($sub == "html") $content = strip_tags($content);
			$content = str_replace(">","&gt;",$content);
			$content = str_replace("<","&lt;",$content);
			$content = str_replace("\r\n", "\r",$content);
			$content = str_replace("\r", "\n",$content);
			$content = preg_replace("/\n{2,}/", "\n\n", $content);
			//$content = str_replace("\n", "<br />", $content);
			if ($write) {
				// Delete phone number
				$content = eregi_replace("([[:digit:]]{11})|([[:digit:]\-]{13})", "", $content);
				// Delete under line
				$content = eregi_replace($del_ereg, "", $content);
				// Delete mac //mac削除
				$content = ereg_replace("Content-type: multipart/appledouble;[[:space:]]boundary=(.*)","",$content);
				// Delete Ads.
				if (is_array($word)) {
					foreach ($word as $delstr)
						$content = str_replace($delstr, "", $content);
				}
				if (strlen($content) > $SurveyCNF['post_limit']) $content = substr($content, 0, $SurveyCNF['post_limit'])."...";
			}
		}else{
			$permission = sprintf("%03d",$blog->permission);
			// Pickup filename
			if (eregi("name=\"?([^\"\n]+)\"?",$m_head, $filereg)) {
				$filename = trim($filereg[1]);
				// Omit the space char between encode strings
				$filename = preg_replace("/\?=[\s]+?=\?/","?==?",$filename);
				while (eregi("(.*)=\?iso-[^\?]+\?B\?([^\?]+)\?=(.*)",$filename,$regs)) {//MIME B
					$filename = $regs[1].base64_decode($regs[2]).$regs[3];
				}
				$filename = strtolower(convert($filename));
			}
			// Decode attached file and save it
			if (eregi($SurveyCNF['subtype'], $sub)){ $deny=0; } else { $deny=1; };
			$ext = end(explode(".",$filename)); //strtolower(
			if (eregi($SurveyCNF['viri'], $ext)) $deny = 1;			
			if (eregi("\nContent-Transfer-Encoding:.*base64", $m_head) && eregi($SurveyCNF['imgtype'].'|'.$SurveyCNF['embedtype'], $sub)) {
				$upfile_localname=$uname."_".time().$permission."_".$filename;		// convert for mbstrings
				$tmp = base64_decode($m_body);
				if (strlen($tmp) < $SurveyCNF['maxbyte'] && !eregi($SurveyCNF['viri'], $filename) && $write) {
					$upfile_localname = cnv_mbstr($upfile_localname);
					$upfile_url = XOOPS_URL.$SurveyCNF['img_dir'].rawurlencode($upfile_localname);
					$upfile_path = XOOPS_ROOT_PATH.$SurveyCNF['img_dir'].$upfile_localname;
					$fp = fopen($upfile_path, "wb");
					fputs($fp, $tmp);
					fclose($fp);
					$attach = $filename;
 					if (eregi($SurveyCNF['imgtype'], $sub)){
						// Thumbs Support ( PHP GD Libraly Required )
						if (eregi($SurveyCNF['thumb_ext'],$upfile_localname)) {
							$size = getimagesize($upfile_path);
							if ($size[0] > $SurveyCNF['w'] || $size[1] > $SurveyCNF['h']) {
								$thumbfilename = thumb_create($upfile_path,$SurveyCNF['w'],$SurveyCNF['h'],XOOPS_ROOT_PATH.$SurveyCNF['thumb_dir']);
								$addimg .= "[url=".$upfile_url."][img align=left]".XOOPS_URL.$SurveyCNF['thumb_dir'].rawurlencode($thumbfilename)."[/img][/url]";
							} else {
								$addimg .= "[img align=left]".$upfile_url."[/img]";
							}
						} else {
							$addimg .= "[img align=left]".$upfile_url."[/img]";
						}
 					} elseif (eregi($SurveyCNF['embedtype'], $sub)){
						$addimg .= "\n<EMBED src=\"".$upfile_url."\" WIDTH=\"".$SurveyCNF['w']."\" HEIGHT=\"".$SurveyCNF['h'].
						"\" autostart=\"false\" controller=\"true\" hspace=\"5\" align=\"left\" alt=\"\"></EMBED>\n";
					}
				} else {
					$write = false; $notice ="Max upload size (".$SurveyCNF['maxbyte'].") Over or Deny Ext.($ext)";
				}
			} elseif ($deny==0) {
				$tmp = base64_decode($m_body);
				$upfile_localname=$uname."_".$filename.".".time().$permission;
				$upfile_url='/'.rawurlencode($upfile_localname);	// XOOPS_UPLOAD_URL.
				$upfile_localname = cnv_mbstr($upfile_localname);		// convert for mbstrings
				$fp = fopen($SurveyCNF['uploads'].$upfile_localname, "wb");
				fputs($fp, $tmp);
				fclose($fp);
				$addfile .= "\n:download:[url=".$SurveyCNF['root']."download.php?url=".$upfile_url."]".$filename."[/url]\n";
				// $addfile .= $m_head.$sub;
			} elseif ( strlen($ext)>0 ) {
				$addfile .= "\nUpload Denied...".$sub." ext ".$ext;
			}
		}
	}
	//$content = $addimg.$content.$addfile;

	if ($imgonly && $attach==""){ $write = false; $notice ="Accept image only";}
	list($old,,,,,) = explode("<>", $lines[0]);
	$id = $old + 1;
	if(trim($subject)=="") $subject = $nosubject;
	$line = "$id<>$now<>$subject<>$from<>$content<>$attach<>\n";
	$cmdstr = ereg_replace(";(.*)","",$cmdstr);		// delete after Semicolon
	$keystr = eregi_replace("&gt;|>",":",$keystr);
	//
	// Survey Control section
	//
	if ($write) {
		$msg = TxtInterface( $keystr, $cmdstr, $content, $from, $now );
		if( $msg ){
			ret_result_mail($from,$keystr,$cmdstr,$subject,$msg);
			print("Command responsed.");
		}else{
			ret_result_mail($from,$keystr,$cmdstr,$subject,"ERROR - TxtInterface");
			print("Error occured.");
		}
	} else {
		ret_result_mail($from,$keystr,$cmdstr,$subject,"Error - ".$notice);
		print("Error - ".$notice);
	}
	if ($write) {
		array_unshift($lines, $line);
		// Save a header info.
		if ($head_save) survey_head_save($id,$head);
	} else {
		// Save a deny mail log
		if ($denylog_save) survey_deny_log($head,$subject,str_replace("<br />","\n",$content),$notice);
	}
}
// Logfile max line
if (count($lines) > $maxline) {
	for ($k=count($lines)-1; $k>=$maxline; $k--) {
		list($id,$tim,$sub,$fro,$tex,$at,) = explode("<>", $lines[$k]);
		if (file_exists($SurveyCNF['uploads'].$at)) @unlink($SurveyCNF['uploads'].$at);
		$lines[$k] = "";
	}
}
// Save to log.
if ($write) {
	$fp = fopen($log, "wb");
	flock($fp, LOCK_EX);
	fputs($fp, implode('', $lines));
	fclose($fp);
} else {
	// Update a timestamp for log file.
	@touch($log);
}
//
// Return to admin
//
if ($admin)
	redirect_header($SurveyCNF['admin'],3,$JUST_POPED);
else
	redirect_header($SurveyCNF['root'],12,$JUST_POPED);
exit;
/******************************************************************************/
// End of main procedure
/******************************************************************************/
/* Send Command */
function _sendcmd($cmd) {
	global $sock,$jump;
	fputs($sock, $cmd."\r\n");
	$buf = fgets($sock, 512);
	if(substr($buf, 0, 3) == '+OK') {
		return $buf;
	} else {
		error_output($buf);
	}
	return false;
}
/* Divide header and body */
function mime_split($data) {
	$part = split("\r\n\r\n", $data, 2);
	$part[0] = ereg_replace("\r\n[\t ]+", " ", $part[0]);
	return $part;
}
/* Pickup email address */
function addr_search($addr) {
	if (eregi("[-!#$%&\'*+\\./0-9A-Z^_`a-z{|}~]+@[-!#$%&\'*+\\/0-9=?A-Z^_`a-z{|}~]+\.[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+", $addr, $fromreg)) {
		return $fromreg[0];
	} else {
		return false;
	}
}
/* Convert to local language (it need a mb function support on php.ini */
function convert($str,$code=_CHARSET) {
	if (function_exists('mb_convert_encoding')) {
		return mb_convert_encoding($str, $code, "auto");
	} elseif (function_exists('JcodeConvert')) {
		return JcodeConvert($str, 0, 2);
	}
	return $str;
}
// Save a denied mail log.
function survey_deny_log($head,$subject,$body,$notice){
	global $denylog;
	$subject = unhtmlentities($subject);
	$body = unhtmlentities($body);
	// Save to file
	$fp = fopen($denylog, "a+b");
	flock($fp, LOCK_EX);
	fputs($fp, "Couse Denied: {$notice}\n\n");
	fputs($fp, "{$head}\n\nSubject: {$subject}\n\n{$body}\n\n\n");
	fclose($fp);
	return;
}
// Save a header information
function survey_head_save($id,$head){
	global $log_dir,$head_prefix;
	// Save to file
	$fp = fopen($log_dir.$head_prefix.$id.".cgi", "wb");
	flock($fp, LOCK_EX);
	fputs($fp, $head);
	fclose($fp);
	return;
}

// Trun back to HTML Entities
function unhtmlentities ($string)
{
	$trans_tbl = get_html_translation_table (HTML_ENTITIES);
	$trans_tbl = array_flip ($trans_tbl);
	return strtr ($string, $trans_tbl);
}

// Output Error
function error_output ($str)
{
	global $jump,$img_mode;
	if ($img_mode)
	{
		header("Content-Type: image/gif");
		readfile('poperror.gif');
	}
	else
	{
		redirect_header($jump,3,$str);
	}
	exit;
}
function TxtInterface( $key='', $cmd = '',  $content, $from, $now ){
	global $xoopsDB;
	global $xoopsUser;

	$debug=0;

	if (!$key) return false;
	$blah = explode("\n", $content);
	reset($blah);
	$l_stack = array();
	while (list($n,$lstr) = each($blah)) {
		$sepline = explode(':', $lstr);
		$sepline[0] = ereg_replace("^&gt;","",$sepline[0]);	// delete replay simbol
		$sepline[0] = trim($sepline[0]);					// delete space
		$sepline[0] = eregi_replace("^q","",$sepline[0]);	// delete question simbol
		if ($debug) print_r($sepline);
		if ($sepline[1]){
			if (preg_match("/[\d]\.\((.*)/", $sepline[1])) {
				// For single choice
				$cmdstr = explode(" ", $sepline[1]);
				while(list ($head,$val) = each($cmdstr)) {
					if ( ereg('(.*)\.\((.*)\)',$val,$ipdat) ){
						if (strlen($ipdat[2])>0)
							$l_stack[$sepline[0]] = $ipdat[1];	//$head;
					}
				}
			} elseif (preg_match("/[\d]\.\[(.*)/", $sepline[1])) {
				// For Multi choice
				$cmdstr = explode(" ", $sepline[1]);
				while(list ($head,$val) = each($cmdstr)) {
					if ( ereg('(.*)\.\[(.*)\]',$val,$ipdat) ){
						if (strlen($ipdat[2])>0){
							$l_stack[$sepline[0]][$ipdat[1]] = $ipdat[1];	//$ipdat[2];
						}
					}
				}
			} else {
				// For Strings
				$cmdstr = $sepline[1];
				if ( ereg('\[(.*)\]',$cmdstr,$ipdat) ){
					$ret = $ipdat[1];
				}else{
					$ret = $cmdstr;
				}
				$l_stack[$sepline[0]] =  $ret;
			}
		}
	}
	if ($debug){
		echo "key=$key<br>";
		echo "cmd=$cmd<br>";
		echo "from=$from<br>";
		echo date("M d Y H:i:s", $now)."<br>";
		print_r($l_stack);
	}
	if (preg_match("/M|Re:M/i",$key,$reg)){
		$msg = member_ctl($cmd,$l_stack,$from,$now);	// Member Ctl.
	}elseif (preg_match("/Q|Re:Q/i",$key,$reg)){
		if ($debug) echo "qreturn_ctl($cmd,$l_stack,$from,$now)";
		$msg = qreturn_ctl($cmd,$l_stack,$from,$now);	// Question return
	}
	return $msg;
}
//
// Member Control Section
//
function member_ctl($cmd, $l_stack, $from, $now) {
	global $xoopsDB;
	$unm = $l_stack['u'];	// User name
	$fnm = $l_stack['f'];	// First name
	$lnm = $l_stack['l'];	// Last name
	$sid = $l_stack['s'];	// Last name
	$exd = $l_stack['d'];	// Expire date
	$exdates= explode("/",$exd);
	$dates = getdate($now);
	$ret = '';
	$sqlDate =sprintf("%04d-%02d-%02d %02d:%02d:%02d",$dates['year'],$dates['mon'],$dates['mday'],$dates['hours'],$dates['minutes'],$dates['seconds']);
	$expDate =sprintf("%04d-%02d-%02d %02d:%02d:%02d",$exdates[0],$exdates[1],$exdates[2],23,59,59);
	switch ($cmd){
		case "NEW": //Entry with User Name.
			$sql = sprintf("select count(*) from %s where username = '%s'",TABLE_RESPONDENT, $unm);
			if( list($num)=$xoopsDB->fetchRow($xoopsDB->query($sql)) ){
				if ( $num > 0 ) return _MD_POP_MNEW_AREADY;
			} else {
				return 'SQL Error';
			}
			$sql = sprintf("insert into %s(username,email,fname,lname,survey_id,changed,expiration) values('%s','%s','%s','%s','%u','%s','%s')"
				,TABLE_RESPONDENT,$unm,$from,$fnm,$lnm,$sid,$sqlDate,$expDate);
			$res = sprintf(_MD_POP_MNEW_ENTRY,$unm);
			break;	// Request From with survey name
	}
	if(!empty($sql)){
		bmsurveyUtils::log($sql);
		if ($ret=$xoopsDB->queryF($sql)) return $res;
		else return 'Error';
	}
}
//
// Member Control Section
//
function qreturn_ctl($cmd, $l_stack, $from, $now) {
	global $xoopsDB;
	$debug=0;
	
	$OneRes = bmsurveyUtils::getXoopsModuleConfig('ONE_RESPONSE');
	$unm    = isset($l_stack['u']) ? $l_stack['u'] : "" ;	// User name
	$ticket = isset($l_stack['t']) ? $l_stack['t'] : "" ;	// Ticket
	$sid    = isset($l_stack['s']) ? $l_stack['s'] : 0 ;	// Survey id
	$rid    = isset($l_stack['r']) ? $l_stack['r'] : 0 ;	// response id
	$dates = getdate($now);
	$ret = '';
	$sqlDate =sprintf("%04d-%02d-%02d %02d:%02d:%02d",$dates['year'],$dates['mon'],$dates['mday'],$dates['hours'],$dates['minutes'],$dates['seconds']);
	if($debug){
		echo "<br>".$cmd;
		print_r ($l_stack);
	}
	switch ($cmd){
		case "INP": //Input data.
			$sql = sprintf("select survey_id,response_id from %s where username='%s' and password='%s'",TABLE_RESPONDENT, $unm, $ticket);
			if ( list($r_sid,$rid)=$xoopsDB->fetchRow($xoopsDB->query($sql)) ){
				if ( !$r_sid || $r_sid!=$sid ) return _MD_POP_QINP_FAILEDLOGIN;
			}
			//echo $sql.$num.$rid;
			if ( $OneRes && $rid ){	// Make Delete response
				return _MD_POP_QINP_DELETEIT."\nDELETE:RID\n".
					"u:".$unm."\n".
					"t:".$ticket."\n".
					"s:".$sid."\n".
					"r:".$rid."\n";
			}
			$rid = resmail_insert($unm,$sid,$l_stack);
			response_commit($rid);
			response_send_email($sid, $rid,$unm,$from);
			
			$sql = sprintf("UPDATE %s SET changed='%s',response_id='%u' WHERE username='%s'", TABLE_RESPONDENT, $sqlDate, $rid, $unm);
			$res = sprintf(_MD_POP_QINP_SUCCEEDED,$unm);
			break;	// Request From with survey name
		case "DEL": //Input data.
			$sql = sprintf("select survey_id,response_id from %s where username='%s' and password='%s'",TABLE_RESPONDENT, $unm, $ticket);
			if ( list($r_sid,$rid)=$xoopsDB->fetchRow($xoopsDB->query($sql)) ){
				if ( !$r_sid || $r_sid!=$sid ) return _MD_POP_QINP_FAILEDLOGIN;
			}
			//echo $sql.$num.$rid;
			if ( $rid ){	// Make Delete response
				response_delete($sid,$rid);
				$sql = sprintf("UPDATE %s SET changed='%s',response_id=0 WHERE username='%s'", TABLE_RESPONDENT, $sqlDate, $unm);
				$res = sprintf(_MD_POP_QDEL_SUCCEEDED,$unm);
			}
			break;	// Request From with survey name
	}
	if(!empty($sql)){
		bmsurveyUtils::log($sql);
		if ($ret=$xoopsDB->queryF($sql)) return $res;
		else return 'Error';
	}
}
// make timestamp in server side timezone to post blog data
// By hoshiyan@hoshiba-farm.com 2004/7/14
function make_timestamp_for_post($datereg) {
	$debug = 0;
	$now = strtotime($datereg[1]);
	if ($now == -1) $now = date('U');
	if ($debug) {
		print("datereg".$datereg[1]);
		print("now".$now);
		echo "Server Timezone = ".date("Y-m-d H:i:s", $now);
		echo "User Timezone = ".formatTimeZone($now, 'l');
	}
	return $now;
}
function ret_result_mail($to,$key,$cmd,$subject,$message='') {
	global $xoopsConfig,$SurveyCNF;

	if (preg_match("/M|Re:M/i",$key,$reg)){
		$keymsg = _MD_POP_KEY_M;
	}elseif (preg_match("/Q|Re:Q/i",$key,$reg)){
		$keymsg = _MD_POP_KEY_Q;
	}elseif (preg_match("/U/i",$key,$reg)){
		$keymsg = _MD_POP_KEY_U;
	}else{
		$keymsg = _MD_POP_KEY_ERR."[".$key."]";
	}
	switch ($cmd){
		case "NEW": $cmdmsg = _MD_POP_CMD_NEW; break;
		case "INP": $cmdmsg = _MD_POP_CMD_INP; break;
		case "DEL": $cmdmsg = _MD_POP_CMD_DEL; break;
	}
	$xoopsMailer =& getMailer();
	$xoopsMailer->useMail();
	$xoopsMailer->setTemplateDir($SurveyCNF['path']."language/".$xoopsConfig['language']."/mail_template/");
	$xoopsMailer->setTemplate("mail_results.tpl");
	$xoopsMailer->setToEmails($to);
	$xoopsMailer->assign("KEY", $keymsg);
	$xoopsMailer->assign("COMMAND", $cmdmsg);
	$xoopsMailer->assign("MESSAGE", $message);
	$xoopsMailer->assign("SITENAME", $xoopsConfig['sitename']);
	$xoopsMailer->assign("ADMINMAIL", $xoopsConfig['adminmail']);
	$xoopsMailer->assign("SITEURL", $xoopsConfig['xoops_url']."/");
	$xoopsMailer->setFromEmail($xoopsConfig['adminmail']);
	$xoopsMailer->setFromName($xoopsConfig['sitename']);
	if (eregi("DELETE:RID",$message)){
		$subj = "Q,DEL;".$subject;
	}else{
		$subj = "Re:".$subject;
	}
	if ( function_exists('mb_encode_mimeheader') )
		$subj = mb_encode_mimeheader( $subj, bmsurveyUtils::get_mailcode(), "B" );
	$xoopsMailer->setSubject($subj);
	$xoopsMailer->send();
}

?>
