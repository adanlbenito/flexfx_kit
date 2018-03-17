const char interface_string[] = "2320464c455846582053544552454f2043414253494d2050726f706572747920496e746572666163650a230a232053746572656f20436162696e65742053696d756c61746f72207573696e6720696d70756c736520726573706f6e7365732e20496d70756c736520726573706f6e73657320746f2075706c6f61640a23206d7573742062652073746f72656420696e206120776176652066696c652028524946462f57415620666f726d61742920616e64206861766520612073616d706c696e67206672657175656e63790a23206f66203438206b487a2e20426f7468206d6f6e6f20616e642073746572656f20736f75726365206461746120697320737570706f727465642e202053746572656f2063616e20616c736f2062650a2320656d706c6f7965642062792073706563696679696e672074776f206d6f6e6f205741562066696c65732e0a230a232050524f50204944202020444952454354494f4e20202020202020204445534352495054494f4e0a2320303030302020202020204269646972656374696f6e616c202020204563686f207468652070726f70657274792076616c756573206261636b20746f2074686520686f73740a2320303030312020202020204269646972656374696f6e616c2020202051756572792f72657475726e2074686520706c6174666f726d207369676e6174757265202833646567466c6578290a2320303030322020202020204269646972656374696f6e616c2020202051756572792f72657475726e207468652064657669636520636f6d70616e79206e616d650a2320303030332020202020204269646972656374696f6e616c2020202051756572792f72657475726e20746865206465766963652070726f64756374206e616d650a2320303030342020202020204269646972656374696f6e616c2020202051756572792f72657475726e20746865206465766963652070726f647563742076657273696f6e2f636f6465730a2320303030352020202020204269646972656374696f6e616c20202020426567696e206669726d7761726520757067726164652c206563686f6564206261636b20746f20686f73740a2320303030362020202020204269646972656374696f6e616c202020204e657874203430206279746573206f66206669726d7761726520696d61676520646174612c206563686f65640a2320303030372020202020204269646972656374696f6e616c20202020456e64206669726d7761726520757067726164652c206563686f6564206261636b20746f20686f73740a232030303038202020202020486f737420746f2044657669636520202053746172742064756d70696e6720746865207465787420696e20746869732066696c650a23203030303920202020202044657669636520746f20486f73742020204e657874203430206279746573206f662070726f706572747920696e7465726661636520746578740a23203030304120202020202044657669636520746f20486f7374202020456e64206f662070726f706572747920696e7465726661636520746578742064756d700a232030303042202020202020486f737420746f204465766963652020205374617274206120636f6e74726f6c6c6572206a61766173637269707420636f64652064756d700a23203030304320202020202044657669636520746f20486f73742020204e657874203430206279746573206f66206a61766173637269707420636f646520746578740a23203030304420202020202044657669636520746f20486f7374202020456e64206f6620636f6e74726f6c6c6572206a61766173637269707420636f646520746578740a2320303030452020202020204269646972656374696f6e616c2020202051756572792f72657475726e20445350207468726561642072756e2d74696d652028617564696f206379636c6573290a2320303030462020202020204269646972656374696f6e616c20202020466f7263652064657669636520726573657420286e6f74206e65656465642061667465722046572075706772616465290a232031303030202020202020486f737420746f2044657669636520202052657475726e20766f6c756d652c746f6e652c70726573657420636f6e74726f6c2073657474696e67730a2320313030312020202020204269646972656374696f6e616c2020202055706461746520636f6e74726f6c7320286f766572726964657320706879736963616c20636f6e74726f6c73290a2320316e30312020202020204269646972656374696f6e616c20202020557020746f203230206368617261637472206e616d6520666f7220707265736574204e2028313c3d4e3c3d39290a2320316e30322020202020204269646972656374696f6e616c20202020426567696e20646174612075706c6f616420666f7220707265736574204e2c20626567696e2075706c6f61642041434b0a2320316e30332020202020204269646972656374696f6e616c2020202046697665204952206461746120776f72647320666f7220707265736574204e206f72206563686f656420646174610a2320316e30342020202020204269646972656374696f6e616c20202020456e6420646174612075706c6f616420666f7220707265736574204e206f7220656e642075706c6f61642041434b0a2320316e30352020202020204269646972656374696f6e616c202020204669727374203230206368617273206f6620646174612066696c65206e616d6520666f7220707265736574204e0a2320316e30362020202020204269646972656374696f6e616c202020204e657874203230206368617273206f6620646174612066696c65206e616d6520666f7220707265736574204e0a2320316e30372020202020204269646972656374696f6e616c202020204c617374203230206368617273206f6620646174612066696c65206e616d6520666f7220707265736574204e0a230a232050726f7065727479206c61796f757420666f7220636f6e74726f6c20286b6e6f62732c2070757368627574746f6e732c20657463292056616c7565732073686f776e206172652033322d6269740a232076616c75657320726570726573656e74656420696e2041534349492f48455820666f726d6174206f7220617320666c6f6174696e6720706f696e742076616c7565732072616e67696e670a232066726f6d202b302e3020757020746f20286e6f7420696e636c7564696e6729202b312e302e0a230a23202b2d2d2d2d2d2d2d2045666665637420706172616d65746572206964656e746966696572202850726f7065727479204944290a23207c0a23207c202020202b2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d20566f6c756d65206c6576656c0a23207c202020207c20202020202b2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d20546f6e652073657474696e670a23207c202020207c20202020207c20202020202b2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2052657365727665640a23207c202020207c20202020207c20202020207c20202020202b2d2d2d2d2d2d2d2d2d2d2d2d2d2d2052657365727665640a23207c202020207c20202020207c20202020207c20202020207c20202020202b2d2d2d2d2d2d2d2d205072657365742073656c656374696f6e202831207468726f7567682039290a23207c202020207c20202020207c20202020207c20202020207c20202020207c2b2d2d2d2d2d2d2d20456e61626c65642028313d7965732c303d6279706173736564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c2b2d2d2d2d2d2d20496e7075744c202028313d706c75676765642c303d756e706c7567676564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c2b2d2d2d2d2d204f75747075744c2028313d706c75676765642c303d756e706c7567676564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c7c2b2d2d2d2d20496e70757452202028313d706c75676765642c303d756e706c7567676564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c7c7c2b2d2d2d204f7574707574522028313d706c75676765642c303d756e706c7567676564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c7c7c7c2b2d2d2045787072657373696f6e2028313d706c75676765642c303d756e706c7567676564290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c7c7c7c7c2b2d2055534220417564696f2028313d616374697665290a23207c202020207c20202020207c20202020207c20202020207c20202020207c7c7c7c7c7c7c7c0a23203130303120302e35303020302e35303020302e35303020302e3530302039313131313131310a230a232050726f7065727479206c61796f757420666f72207072657365742064617461206c6f6164696e6720286c6f6164696e672049522064617461292e2056616c7565732073686f776e206172650a232033322d6269742076616c75657320726570726573656e74656420696e2041534349492f48455820666f726d61742e0a230a23202b2d2d2d2d2d2d2d2d2d2d2045666665637420706172616d65746572206964656e746966696572202850726f7065727479204944290a23207c0a23207c2b2d2d2d20507265736574206e756d626572202831207468726f7567682039290a23207c7c0a2320316e303220302030203020302030202320426567696e2049522064617461206c6f6164696e6720666f7220707265736574204e0a2320316e30332041204220432044204520232046697665206f6620746865206e657874204952206461746120776f72647320746f206c6f616420696e746f20707265736574204e0a2320316e303420302030203020302030202320456e642049522064617461206c6f6164696e6720666f7220707265736574204e0a";
const char controller_string[] = "66756e6374696f6e20666c657866785f637265617465282074616720290a7b0a202020207661722078203d2022223b0a0a2020202078202b3d20223c703e223b0a2020202078202b3d202253746572656f20436162696e65742053696d756c61746f72207573696e6720696d70756c736520726573706f6e7365732e20496d70756c736520726573706f6e73657320746f2075706c6f6164206d75737420626520223b0a2020202078202b3d202273746f72656420696e206120776176652066696c652028524946462f57415620666f726d61742920616e64206861766520612073616d706c696e67206672657175656e6379206f66203438206b487a2e20426f746820223b0a2020202078202b3d20226d6f6e6f20616e642073746572656f20736f75726365206461746120697320737570706f727465642e2053746572656f2063616e20616c736f20626520656d706c6f7965642062792073706563696679696e672074776f20223b0a2020202078202b3d20227365706172617465206d6f6e6f205741562066696c65732e223b0a2020202078202b3d20223c2f703e223b0a2020202078202b3d20223c7461626c6520636c6173733d27666c65786678273e223b0a2020202078202b3d20223c74686561643e223b0a2020202078202b3d20223c74723e3c74683e5072657365743c2f74683e3c74683e4c6566742f4d6f6e6f3c2f74683e3c74683e52696768742f53746572656f3c2f74683e3c74683e46696c65204e616d652873293c2f74683e3c2f74723e223b0a2020202078202b3d20223c2f74686561643e223b0a0a2020202078202b3d20223c74626f64793e223b0a20202020666f7228207661722070203d20313b2070203c3d20393b202b2b702029207b0a2020202078202b3d20223c74723e3c746420636c6173733d27707265736574272069643d27222b7461672b225f707265736574222b702b22273e222b702b223c2f74643e223b0a2020202078202b3d20223c74643e3c696e70757420747970653d2766696c6527207374796c653d27646973706c61793a6e6f6e65272069643d27222b7461672b225f696e707574222b702b224c272f3e3c627574746f6e2069643d27222b7461672b225f627574746f6e222b702b224c273e53656c6563742049523c2f627574746f6e3e3c2f74643e223b0a2020202078202b3d20223c74643e3c696e70757420747970653d2766696c6527207374796c653d27646973706c61793a6e6f6e65272069643d27222b7461672b225f696e707574222b702b2252272f3e3c627574746f6e2069643d27222b7461672b225f627574746f6e222b702b2252273e53656c6563742049523c2f627574746f6e3e3c2f74643e223b0a2020202078202b3d20223c74643e3c646976207374796c653d27646973706c61793a696e6c696e652d626c6f636b273e223b0a2020202078202b3d20223c6469762069643d27222b7461672b225f74657874222b702b224c273e43656c657374696f6e204731324820416e6e20313532204f70656e20526f6f6d2e7761763c2f6469763e223b0a2020202078202b3d20223c6469762069643d27222b7461672b225f74657874222b702b2252273e43656c657374696f6e204731324820416e6e20313532204f70656e20526f6f6d2e7761763c2f6469763e223b0a2020202078202b3d20223c2f6469763e3c2f74643e223b2078202b3d20223c2f74723e223b207d0a2020202078202b3d20223c2f74626f64793e3c2f7461626c653e223b0a0a2020202072657475726e20783b0a7d0a0a66756e6374696f6e20666c657866785f696e697469616c697a65282074616720290a7b0a20202020666f7228207661722069203d20313b2069203c3d20393b202b2b6920290a202020207b0a202020202020202024287461672b225f707265736574222b69292e6f6e636c69636b20202020203d205f6f6e5f63616273696d5f73656c6563743b0a202020202020202024287461672b225f627574746f6e222b692b224c22292e6f6e636c69636b203d205f6f6e5f63616273696d5f627574746f6e3b0a202020202020202024287461672b225f627574746f6e222b692b225222292e6f6e636c69636b203d205f6f6e5f63616273696d5f627574746f6e3b0a202020202020202024287461672b225f696e707574222b692b224c22292e6f6e6368616e6765203d205f6f6e5f63616273696d5f696e7075743b0a202020202020202024287461672b225f696e707574222b692b225222292e6f6e6368616e6765203d205f6f6e5f63616273696d5f696e7075743b0a202020207d0a2020202072657475726e205f6f6e5f70726f70657274795f72656365697665643b0a7d0a0a66756e6374696f6e205f6f6e5f63616273696d5f73656c65637428206576656e7420290a7b0a2020202076617220746167203d20666c657866785f6765745f74616728206576656e742e7461726765742e696420293b0a0a20202020696628206576656e742e7461726765742e696e6e657248544d4c5b305d203d3d20275b27202920707265736574203d207061727365496e7428206576656e742e7461726765742e696e6e657248544d4c5b315d20293b0a20202020656c736520707265736574203d207061727365496e7428206576656e742e7461726765742e696e6e657248544d4c20293b0a0a20202020706172656e74203d2024287461672b225f707265736574222b707265736574292e706172656e744e6f64652e706172656e744e6f64653b0a20202020666f7228207661722069203d20313b2069203c3d20393b202b2b69202920706172656e742e6368696c6472656e5b692d315d2e6368696c6472656e5b305d2e696e6e657248544d4c203d20693b0a20202020706172656e742e6368696c6472656e5b7072657365742d315d2e6368696c6472656e5b305d2e696e6e657248544d4c203d20225b22202b20707265736574202b20225d223b0a0a202020202f2f666c657866785f73656e645f70726f706572747928207461672c2070726f706572747920293b0a7d0a0a66756e6374696f6e205f6f6e5f63616273696d5f696e70757428206576656e7420290a7b0a2020202076617220746167202020203d20666c657866785f6765745f74616728206576656e742e7461726765742e696420293b0a2020202076617220756e69742020203d207061727365496e7428206576656e742e7461726765742e69645b287461672b225f696e70757422292e6c656e6774682b305d20293b0a2020202076617220707265736574203d207061727365496e7428206576656e742e7461726765742e69645b287461672b225f696e70757422292e6c656e6774682b315d20293b0a2020202076617220736964652020203d20202020202020202020206576656e742e7461726765742e69645b287461672b225f696e70757422292e6c656e6774682b325d3b0a202020207661722066696c652020203d2024287461672b225f696e707574222b756e69742b22222b7072657365742b73696465292e66696c65735b305d3b0a0a2020202024287461672b225f74657874222b756e69742b22222b7072657365742b73696465292e74657874436f6e74656e74203d2066696c652e6e616d653b0a0a2020202076617220726561646572203d206e65772046696c6552656164657228293b0a202020207265616465722e6f6e6c6f6164203d2066756e6374696f6e2865290a202020207b0a20202020202020207661722073616d706c6573203d20666c657866785f776176655f746f5f73616d706c657328206e65772055696e7438417272617928207265616465722e726573756c742029293b0a2020202020202020636f6e736f6c652e6c6f67282073616d706c65732e6c656e67746820293b0a0a2020202020202020766172206f6666736574203d20303b0a20202020202020207768696c6528206f6666736574203c20313230302029207b0a2020202020202020202020206966282073616d706c65732e6c656e677468203e3d20342029207b0a202020202020202020202020202020207661722070726f7065727479203d205b20307830313031383030302b6f66667365742f352c2073616d706c65735b305d2c73616d706c65735b315d2c73616d706c65735b325d2c73616d706c65735b335d2c73616d706c65735b345d205d3b0a2020202020202020202020202020202073616d706c6573203d2073616d706c65732e736c69636528203520293b0a20202020202020202020202020202020666c657866785f70726f7065727479203d205b302c302c302c302c302c305d3b0a20202020202020202020202020202020666c657866785f73656e645f70726f706572747928207461672c2070726f706572747920293b0a2020202020202020202020207d0a2020202020202020202020207768696c652820312029207b2069662820666c657866785f70726f70657274795b305d203d3d20307830313031383030302b6f66667365742f35202920627265616b3b207d0a2020202020202020202020206f6666736574202b3d20353b0a20202020202020207d0a202020207d0a202020207265616465722e7265616441734172726179427566666572282066696c6520293b0a7d0a0a66756e6374696f6e205f6f6e5f63616273696d5f627574746f6e28206576656e7420290a7b0a2020202076617220746167202020203d20666c657866785f6765745f74616728206576656e742e7461726765742e696420293b0a2020202076617220707265736574203d207061727365496e7428206576656e742e7461726765742e69645b287461672b225f627574746f6e22292e6c656e6774682b305d20293b0a2020202076617220736964652020203d20202020202020202020206576656e742e7461726765742e69645b287461672b225f627574746f6e22292e6c656e6774682b315d3b0a2020202024287461672b225f696e707574222b7072657365742b22222b73696465292e636c69636b28293b0a7d0a0a66756e6374696f6e205f6f6e5f70726f70657274795f7265636569766564282070726f706572747920290a7b0a7d0a0a66756e6374696f6e205f6f6e5f6669726d776172655f737461747573282073746174757320290a7b0a7d0a0a";
