#include "exam.h"
#include <cln/dfloat.h>
#include <cln/dfloat_io.h>

static minus_test dfloat_minus_tests[] = {

{ "-0.011326914400453525d0", "-0.6668141757661364d0",
  "0.6554872613656829d0" },

{ "-0.46185382764946437d0", "0.7488210697846337d0",
  "-1.2106748974340982d0" },

{ "-0.35834120541234993d0", "-0.30919976341834987d0",
  "-0.04914144199400006d0" },

{ "0.44705025064976966d0", "-0.9277893553610955d0",
  "1.3748396060108652d0" },

{ "-0.47647537517067917d0", "0.29158058381073604d0",
  "-0.7680559589814152d0" },

{ "-0.021697999002707746d0", "0.1779871773524142d0",
  "-0.19968517635512195d0" },

{ "0.4179484378019861d0", "9.9990307469939d9",
  "-9.999030746575953d9" },

{ "-0.7475415524823718d0", "1.3993312799214797d9",
  "-1.3993312806690214d9" },

{ "0.2519442433861928d0", "-6.699632771871848d9",
  "6.699632772123793d9" },

{ "-0.5124988631497671d0", "2.7959244812290273d9",
  "-2.795924481741526d9" },

{ "-0.6870193827604301d0", "4.851102442573468d9",
  "-4.851102443260488d9" },

{ "0.7609656780357723d0", "7.481252865855436d8",
  "-7.481252858245779d8" },

{ "-0.6301276042170191d0", "-7.099314875214215d-11",
  "-0.630127604146026d0" },

{ "-0.4139053484357884d0", "-2.897413526398709d-11",
  "-0.41390534840681426d0" },

{ "-0.6944623060197281d0", "-3.291569879873739d-11",
  "-0.6944623059868125d0" },

{ "-0.2057822500703933d0", "3.6505182026159854d-11",
  "-0.2057822501068985d0" },

{ "-0.8792706674467908d0", "8.094527736950817d-11",
  "-0.8792706675277361d0" },

{ "-0.6888184243601332d0", "9.127622796988807d-11",
  "-0.6888184244514094d0" },

{ "-0.980711030497252d0", "8.752272461345245d19",
  "-8.752272461345245d19" },

{ "0.8035082489836539d0", "-3.903355151264917d19",
  "3.903355151264917d19" },

{ "-0.7537841372394811d0", "-5.879942447417834d19",
  "5.879942447417834d19" },

{ "-0.6877475951546845d0", "-2.3972266191169642d19",
  "2.3972266191169642d19" },

{ "-0.43128282112433525d0", "-5.422824998003439d19",
  "5.422824998003439d19" },

{ "0.29538116818276694d0", "1.1291858990580939d19",
  "-1.1291858990580939d19" },

{ "0.9166687388673976d0", "6.395175407123937d-21",
  "0.9166687388673976d0" },

{ "0.41840538498193025d0", "-2.6655662412599155d-21",
  "0.41840538498193025d0" },

{ "-0.8036940092501853d0", "6.7473779576832565d-21",
  "-0.8036940092501853d0" },

{ "0.8555054025209989d0", "-7.939970418096797d-21",
  "0.8555054025209989d0" },

{ "0.3365495704567003d0", "8.694519827555395d-21",
  "0.3365495704567003d0" },

{ "-0.7430322011471231d0", "7.430332379292914d-22",
  "-0.7430322011471231d0" },

{ "5.102372414731216d9", "-0.5073635765350494d0",
  "5.10237241523858d9" },

{ "4.629827365822252d9", "0.6534380055543355d0",
  "4.629827365168815d9" },

{ "7.218192507117569d9", "0.9781542046565127d0",
  "7.218192506139415d9" },

{ "6.595760326622413d8", "0.7339510561932947d0",
  "6.595760319282902d8" },

{ "7.191166637703489d9", "0.80792475493853d0",
  "7.191166636895564d9" },

{ "-7.95531405213956d9", "0.5353636841430115d0",
  "-7.955314052674924d9" },

{ "5.438904545553836d8", "6.533536518165114d9",
  "-5.989646063609731d9" },

{ "-7.389650313101625d8", "-9.983943153365381d9",
  "9.244978122055218d9" },

{ "8.364404619492165d9", "-7.600563055115287d9",
  "1.5964967674607452d10" },

{ "2.070813748323649d9", "6.421052769114957d9",
  "-4.350239020791307d9" },

{ "-2.8555256820439434d9", "-3.4077342921686625d8",
  "-2.514752252827077d9" },

{ "9.147878229420991d8", "8.439982790150545d9",
  "-7.5251949672084465d9" },

{ "-4.315772980070098d9", "-6.48869466068404d-11",
  "-4.315772980070098d9" },

{ "-3.5186299785635023d9", "3.990046539849716d-11",
  "-3.5186299785635023d9" },

{ "2.5645532837267537d9", "8.566645694205622d-13",
  "2.5645532837267537d9" },

{ "6.145110896031829d9", "-9.242734002954773d-11",
  "6.145110896031829d9" },

{ "-6.6836855975624d9", "9.117930361283473d-11",
  "-6.6836855975624d9" },

{ "-1.7472828462085754d8", "-5.125838712019503d-11",
  "-1.7472828462085754d8" },

{ "9.05675399397055d9", "9.086705650502484d19",
  "-9.08670564959681d19" },

{ "-5.834806594586836d9", "9.981576053842906d19",
  "-9.981576054426386d19" },

{ "3.047010922754272d9", "1.1715352070471352d19",
  "-1.1715352067424342d19" },

{ "7.294295638574767d9", "2.845702947515113d19",
  "-2.8457029467856835d19" },

{ "8.264143132493019d9", "-1.6322956072452289d19",
  "1.6322956080716431d19" },

{ "-9.597823287256088d9", "3.954126758718671d19",
  "-3.954126759678453d19" },

{ "3.229389511771705d9", "-4.329831377266493d-21",
  "3.229389511771705d9" },

{ "6.897089200279753d9", "2.4428208790287663d-21",
  "6.897089200279753d9" },

{ "2.3579775300187545d9", "4.729400988996349d-21",
  "2.3579775300187545d9" },

{ "1.6718929117460046d9", "5.8162277016717065d-21",
  "1.6718929117460046d9" },

{ "2.537177500868296d9", "1.4856605280697543d-21",
  "2.537177500868296d9" },

{ "6.117674696930935d9", "-1.6187214719634357d-21",
  "6.117674696930935d9" },

{ "4.1877888304549216d-11", "-0.06920550501017497d0",
  "0.06920550505205286d0" },

{ "9.61054846124015d-11", "0.885309193732889d0",
  "-0.8853091936367835d0" },

{ "2.5559085051828467d-11", "-0.8112181469812297d0",
  "0.8112181470067888d0" },

{ "-1.4549570208293283d-12", "-0.5049325945871657d0",
  "0.5049325945857107d0" },

{ "-7.091628047158497d-11", "0.61946884965934d0",
  "-0.6194688497302563d0" },

{ "2.877466355456826d-11", "0.4496491857374d0",
  "-0.44964918570862533d0" },

{ "1.3041612488449928d-12", "5.408018587130755d9",
  "-5.408018587130755d9" },

{ "-5.379752339715717d-11", "-4.009594691514288d9",
  "4.009594691514288d9" },

{ "7.023042501342336d-12", "-3.4153434285746374d9",
  "3.4153434285746374d9" },

{ "6.968174934871611d-11", "4.713087404332662d9",
  "-4.713087404332662d9" },

{ "-5.153562653896506d-11", "-8.44732228013254d8",
  "8.44732228013254d8" },

{ "-8.424177457818745d-11", "1.6817117809824567d9",
  "-1.6817117809824567d9" },

{ "3.374755984316538d-11", "8.893678266883364d-11",
  "-5.5189222825668264d-11" },

{ "-8.684123447823306d-11", "-7.888825869147879d-11",
  "-7.952975786754267d-12" },

{ "7.788477523205632d-11", "1.741674745286914d-11",
  "6.046802777918718d-11" },

{ "6.546622477606044d-11", "-4.7719651007530584d-11",
  "1.1318587578359101d-10" },

{ "-1.8595152377503265d-11", "5.7288738553553045d-11",
  "-7.588389093105631d-11" },

{ "-8.184033550427558d-11", "-8.834399228929296d-11",
  "6.503656785017376d-12" },

{ "5.749469292140762d-11", "7.493129199779113d19",
  "-7.493129199779113d19" },

{ "-5.2285095120702066d-11", "-2.0611179974216552d19",
  "2.0611179974216552d19" },

{ "-8.84727820032067d-11", "4.7423077384022024d19",
  "-4.7423077384022024d19" },

{ "3.437676989338625d-11", "-3.5368755480277647d19",
  "3.5368755480277647d19" },

{ "2.2665031619145437d-11", "-6.072845659234921d19",
  "6.072845659234921d19" },

{ "-8.429070146313393d-11", "5.134329153614969d18",
  "-5.134329153614969d18" },

{ "-9.009531819191212d-11", "2.301790665456671d-22",
  "-9.00953181921423d-11" },

{ "-2.706942469371907d-11", "9.282350542107287d-21",
  "-2.706942470300142d-11" },

{ "5.358266626996117d-11", "-4.409057695582885d-22",
  "5.358266627040208d-11" },

{ "-7.189537285608088d-11", "9.569273217393917d-21",
  "-7.189537286565016d-11" },

{ "-4.160295905335358d-11", "5.930867524794025d-21",
  "-4.160295905928445d-11" },

{ "6.7922062777334035d-12", "-7.747524338474154d-22",
  "6.792206278508156d-12" },

{ "-9.038821102045805d19", "0.04779131019959271d0",
  "-9.038821102045805d19" },

{ "2.2020595055495963d19", "-0.424631558292516d0",
  "2.2020595055495963d19" },

{ "-8.164003027214308d19", "0.6832198147365239d0",
  "-8.164003027214308d19" },

{ "-3.878233560364984d19", "-0.28756619113600546d0",
  "-3.878233560364984d19" },

{ "7.0829003521450525d19", "-0.6071548125948544d0",
  "7.0829003521450525d19" },

{ "5.968540808784698d19", "0.7674294173432648d0",
  "5.968540808784698d19" },

{ "-2.2143621795153547d19", "-2.443529365769125d9",
  "-2.2143621792710017d19" },

{ "-9.77092538926342d18", "5.903189771537687d8",
  "-9.77092538985374d18" },

{ "9.974714452399537d19", "-6.980456691485629d9",
  "9.974714453097582d19" },

{ "1.7428950527159094d18", "3.68843657888816d9",
  "1.742895049027473d18" },

{ "-1.1094381875350845d19", "-7.157723640671709d9",
  "-1.1094381868193122d19" },

{ "-3.638795590369631d19", "6.9246542750294075d9",
  "-3.6387955910620963d19" },

{ "-5.66543282261991d19", "-5.1005028153082024d-11",
  "-5.66543282261991d19" },

{ "-3.901527864456216d19", "-1.064153465992923d-12",
  "-3.901527864456216d19" },

{ "1.1477489418879848d19", "3.327888063907735d-11",
  "1.1477489418879848d19" },

{ "3.508978072054437d19", "9.238453417997638d-11",
  "3.508978072054437d19" },

{ "-4.7642024461416964d19", "-4.758309941438892d-11",
  "-4.7642024461416964d19" },

{ "-8.307715835429606d19", "3.313910202186439d-11",
  "-8.307715835429606d19" },

{ "2.704675010192592d18", "-2.6840207147078365d19",
  "2.954488215727096d19" },

{ "-9.860969100714668d18", "-4.719594638795429d19",
  "3.7334977287239614d19" },

{ "7.87799781828944d18", "-6.657221298850535d19",
  "7.44502108067948d19" },

{ "-3.3937781740759863d19", "4.783805995045389d19",
  "-8.177584169121376d19" },

{ "-1.0747572720102216d19", "-1.7144708598072445d19",
  "6.397135877970229d18" },

{ "1.3938845733158445d19", "5.604369854609131d19",
  "-4.210485281293287d19" },

{ "6.0938348303695315d19", "1.1005522580049531d-21",
  "6.0938348303695315d19" },

{ "-2.4870844028694925d19", "1.5391650322730598d-22",
  "-2.4870844028694925d19" },

{ "7.323118607079343d19", "6.637280375859432d-21",
  "7.323118607079343d19" },

{ "-4.181201584825501d19", "4.768935182006663d-21",
  "-4.181201584825501d19" },

{ "4.1225910279381205d19", "6.117191687463543d-21",
  "4.1225910279381205d19" },

{ "6.438313875980151d17", "-1.4883489002691529d-21",
  "6.438313875980151d17" },

{ "-4.573961206963222d-21", "0.3586300020381973d0",
  "-0.3586300020381973d0" },

{ "7.74206782371325d-22", "0.23168389210368656d0",
  "-0.23168389210368656d0" },

{ "8.572446613640605d-21", "0.6114581963443891d0",
  "-0.6114581963443891d0" },

{ "-8.539467934859551d-21", "0.33474735899049d0",
  "-0.33474735899049d0" },

{ "-5.55811309570968d-21", "-0.9637216018651454d0",
  "0.9637216018651454d0" },

{ "-6.705839413964189d-21", "0.3787619614522374d0",
  "-0.3787619614522374d0" },

{ "1.338539206480238d-22", "6.683968625235106d9",
  "-6.683968625235106d9" },

{ "-9.64078167549023d-21", "3.291420859310843d9",
  "-3.291420859310843d9" },

{ "-9.26536204591093d-22", "2.9839295142529476d8",
  "-2.9839295142529476d8" },

{ "-3.647737608953592d-21", "6.115300020921433d8",
  "-6.115300020921433d8" },

{ "1.4069763806331204d-21", "-1.183109060480878d9",
  "1.183109060480878d9" },

{ "-6.0037865798761924d-21", "-7.442246743849378d9",
  "7.442246743849378d9" },

{ "-5.994118986299138d-21", "-9.091558282012836d-11",
  "9.091558281413425d-11" },

{ "6.969393585974241d-21", "3.435352867093995d-11",
  "-3.435352866397056d-11" },

{ "-6.278554484817533d-22", "-4.7211920270841604d-11",
  "4.721192027021375d-11" },

{ "-8.603262886304741d-21", "1.7296517702077242d-11",
  "-1.7296517710680505d-11" },

{ "4.104502790901735d-21", "-4.8473213720301105d-11",
  "4.847321372440561d-11" },

{ "-4.449725859444968d-21", "-8.944265568403936d-11",
  "8.944265567958964d-11" },

{ "4.828216540804827d-21", "-1.1712152029346877d19",
  "1.1712152029346877d19" },

{ "-5.65034940464881d-21", "-9.445303840982011d19",
  "9.445303840982011d19" },

{ "-7.24107519738777d-21", "2.340578690102746d19",
  "-2.340578690102746d19" },

{ "1.7659593956231534d-21", "-8.048768257390671d18",
  "8.048768257390671d18" },

{ "-3.0538518255248124d-21", "8.834631867521575d19",
  "-8.834631867521575d19" },

{ "8.57952908388053d-21", "-5.730742870111307d19",
  "5.730742870111307d19" },

{ "-4.5090103564928485d-21", "1.8907114777916313d-21",
  "-6.399721834284479d-21" },

{ "-3.8487625143236447d-22", "5.354282198078924d-21",
  "-5.739158449511288d-21" },

{ "2.6660110440404615d-22", "3.833744224501756d-22",
  "-1.1677331804612944d-22" },

{ "-7.503762004261027d-22", "-9.623906576475644d-21",
  "8.873530376049542d-21" },

{ "-9.113431042260725d-21", "-3.5516521546085545d-21",
  "-5.56177888765217d-21" },

{ "-3.4813735333296525d-21", "-2.6602650182385188d-21",
  "-8.211085150911337d-22" },

};