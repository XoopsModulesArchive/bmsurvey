<?php
// $Id$
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
// The name of this module
define("_MI_SURVEY_NAME","ＢＭサーベイ");
define("_MI_SURVEY_ADMIN","管理画面");
// A brief description of this module
define("_MI_SURVEY_BNAME1","入力フォーム");
define("_MI_SURVEY_DESC","Bluemoon.Multi-Survey");

// Names of blocks for this module
define('_MI_MSURVEY_RESPONDENT','回答者の編集');
define('_MI_MSURVEY_CASTSURVEY','アンケート送信');
define('_MI_MSURVEY_RECIEVECHECK','アンケート受信');
define('_MI_MSURVEY_RESISTER','登録メール送信');
define('_MI_MSURVEY_STATUS','ステータス・チェック');
define('_MI_MSURVEY_FILECHARSET', '添付ファイル名称コード');
define('_MI_MSURVEY_FILECHARSET_DESC', 'サーバ保存時のファイル名に使う文字コードを設定します(SJIS,UTF-8,EUC-JP等)');
define('_MI_MSURVEY_CSVCHARSET', 'CSVファイル名称コード');
define('_MI_MSURVEY_CSVCHARSET_DESC', 'CSVダウンロード時にコンバートする文字コードを設定します(SJIS,UTF-8,EUC-JP等)');
define('_MI_MSURVEY_CSVADDNUM', 'CSV出力のフィールド名欄番号標記');
define('_MI_MSURVEY_CSVADDNUM_DESC', 'CSV出力のフィールド名欄に通し番号を振ります。');
define('_MI_MSURVEY_CHOICEOPT','CSV出力の選択項目タイプ');
define('_MI_MSURVEY_CHOICEOPT_DESC','選択項目の出力を文字列か番号（並び順）で行うかを選択します。');
define('_MI_MSURVEY_CSVOTHERF','CSV出力時の!otherフォーマット');
define('_MI_MSURVEY_CSVOTHERF_DESC','選択項目その他で入力された値を出力する時のフォーマットを指定します。');
define('_MI_MSURVEY_MAILSERVER', 'メール・サーバ');
define('_MI_MSURVEY_MAILSERVER_DESC', '受信用メールのPOP3サーバを設定します');
define('_MI_MSURVEY_MAILUSER', 'メール・ユーザー');
define('_MI_MSURVEY_MAILUSER_DESC', 'サーベイ用メールアドレスのユーザー名を設定します');
define('_MI_MSURVEY_MAILPWD', 'メール・パスワード');
define('_MI_MSURVEY_MAILPWD_DESC', 'サーベイ用メールアドレスのパスワードを設定します');
define('_MI_MSURVEY_MAILADDR', 'メール・アドレス');
define('_MI_MSURVEY_MAILADDR_DESC', 'サーベイ用メールアドレスを設定します');
define('_MI_MSURVEY_CASTKEY', '質問配信用認証コード');
define('_MI_MSURVEY_CASTKEY_DESC', 'cast.php を実行するときのkey文字列を設定します');
define('_MI_MSURVEY_MANAGEGROUP', '管理メニュー利用グループ');
define('_MI_MSURVEY_MANAGEGROUP_DESC', '管理メニューを利用できるグループを設定します');
define('_MI_MSURVEY_MGPSTATUS', '管理グループの権限');
define('_MI_MSURVEY_MGPSTATUS_DESC', '管理グループによる編集・アクティベート・終了を許可します');
define('_MI_MSURVEY_BLOCKLIST', 'ブロックリスト数');
define('_MI_MSURVEY_BLOCKLIST_DESC', 'ブロック中に表示するアンケートの数を設定します');
define('_MI_MSURVEY_ADDINFO', '回答に付加情報を付加');
define('_MI_MSURVEY_ADDINFO_DESC', '回答に付加情報を含めてメール送信します');
define('_MI_MSURVEY_ADDUSAGE', '質問に使い方を付加');
define('_MI_MSURVEY_ADDUSAGE_DESC', '質問メールに使い方情報を含めてメール送信します');
define('_MI_MSURVEY_ONERESPONSE', '回答を1回に限定');
define('_MI_MSURVEY_ONERESPONSE_DESC', '１つの質問メールに対し１回だけ回答できるようにします');
define('_MI_MSURVEY_RESETRADIOBUTTON', 'ラジオボタンの選択解除');
define('_MI_MSURVEY_RESETRADIOBUTTON_DESC', 'ラジオボタンを選択解除できるようにします');
define('_MI_MSURVEY_RESULTRANK', '結果表示画面におけるレート表示方法');
define('_MI_MSURVEY_RESULTRANK_DESC', '平均グラフかカウント表示かを選択します');
?>