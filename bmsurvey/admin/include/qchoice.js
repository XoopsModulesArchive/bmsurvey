<!--
    // Make a Top selection
    var bunruiA = new Array(
    	"Archived",
    	"Edit",
    	"Test",
    	"Active",
    	"Finished"
    );
 
    // ���̕���(����A���Ƃ̕���B���X�g)���`
    var bunruiB = new Array();
    bunruiB["���"]= new Array("���Ⴊ����","�ɂ񂶂�","�s�[�}��");
    bunruiB["�ʕ�"]= new Array("�X�C�J(1/4)","�I�����W","������");
    bunruiB["��"]  = new Array("�ؓ�(100g)","����(100g)","�r��(100g)");
    bunruiB["��"]  = new Array("�T���}(1��)","�A�W(1��)","���炷(���p�b�N)");
 
    // ����A�̑I�����X�g���쐬
    createSelection( form1.elements['sel_bunruiA'], "(type)", bunruiA, bunruiA)
 
    ////////////////////////////////////////////////////
    //
    // �I���{�b�N�X�ɑI������ǉ�����֐�
    //    ����: ( select�I�u�W�F�N�g, value�l, text�l)
    function addSelOption( selObj, myValue, myText )
    {
        selObj.length++;
        selObj.options[ selObj.length - 1].value = myValue ;
        selObj.options[ selObj.length - 1].text  = myText;
 
    }
    /////////////////////////////////////////////////////
    //
    //    �I�����X�g�����֐� 
    //    ����: ( select�I�u�W�F�N�g, ���o��, value�l�z�� , text�l�z�� )
    //
    function createSelection( selObj, midashi, aryValue, aryText )
    {
        selObj.length = 0;
        addSelOption( selObj, midashi, midashi);
        // ������
        for( var i=0; i < aryValue.length; i++)
        {
            addSelOption ( selObj , aryValue[i], aryText[i]);
        }
    }
    ///////////////////////////////////////////////////
    //
    //     ����A���I�����ꂽ�Ƃ��ɌĂяo�����֐�
    //
    function selectBunruiA(obj)
    {
        // �I�����𓮓I�ɐ���
        createSelection(form1.elements['sel_bunruiB'], "(�i��)", 
                bunruiB[obj.value], bunruiB[obj.value]);
 
    }
    /////////////////////////////////////////////////
    // submit�O�̏���
    function gettext(form){ 
        var a = form1.sel_bunruiA.value;   // ����1
        var b = form1.sel_bunruiB.value;   // ����2
        // AND�łȂ���
        form1.elements['search'].value = a+' AND '+b;
        alert(form1.elements['search'].value );
    } 
 
//-->
