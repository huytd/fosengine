# Introduction #

Giới thiệu về SGF


# Details #

Ghi chú: Chi tiết cụ thể về các hàm có thể tham khảo source code, comment, hoặc dùng doxygen để tạo ra help file.

**Quản lý bộ nhớ trong sgf**
sgfObject: base class cho gần như mọi class của sgf. Nó chỉ đơn giản là giúp cho việc count reference đơn giản hơn với addRef và decRef.
Khi refCounter về 0(tức là không ai thèm cái object đó nữa thì delete). Yêu cầu là khi class kế thừa từ sgfObject hoặc các con của nó thì
destructor của nó phải là virtual do nó bị delete từ base class.

sgfPtr: smart pointer của hệ thống. Được sử dụng thay cho con trỏ thông thường nếu sử dụng class kế thừa từ sgfObject. Vd: có class A.
Thay vì:
A**myA=new A(...);
ta dùng:
sgfPtr**

&lt;A&gt;

 myA=new A(...);
nếu class tên là B thì
sgfPtr

&lt;B&gt;

 myB=new B(...);

Từ đó, sẽ không phải lo delete cái myA nữa. Nếu ko tin, hãy thử:
myA=NULL;
Và cho destructor(virtual) của A print một cái gì đó, sẽ thấy message hiện ngay khi myA=NULL.
Việc sử dụng sgfPtr giúp cho người lập trình không cần quan tâm đến việc quản lý bộ nhớ, ko lo memory leak và cũng ko sợ delete quá sớm.

Chú ý: sgfPtr có thể được sử dụng như một con trỏ bình thường , sgfPtr

&lt;A&gt;

 không khác gì A**trong hầu hết mọi trường hợp.
Các phép -> ,** , & đều hoạt động tốt. getPtr() chỉ cần sử dụng khi kiểm tra null.
Kiểu giá trị được thay đổi tự động. Tức là nếu có hàm cần tham số là A**, bạn vẫn có thể đưa sgfPtr**

&lt;A&gt;

 vào mà không có lỗi.
Tuy nhiên:
-Khi kiểm tra null thì dùng if(myA.getPtr()) thay vì if(myA)
-Khi cần dynamic\_cast thì dùng myA.dynamicCast<...>();
-Khi cần static\_cast thì dùng myA.staticCast<...>();

**Các khái niệm trong SGF**
Game: một object thể hiện trò chơi. Chỉ có một instance của nó được tạo. Constructor của nó khởi động mọi thứ. Destructor giải phóng. Method run chạy game.

Core: nơi tập hợp các thành phần của framework, gồm có :
-scriptVM: máy ảo dành cho script
-entityManager: object quản lý entity
-graphicDevice: irrlicht device
-fileSystem: hệ thống file ảo
-guiSkin
-inputManager: quản lý bàn phím, chuột, joystick(có thể có trong tương lai)
-các frameEvent
Delegate: Là một object, nó được sử dụng với mục đích: biến method của object hoặc một function thành tham số cho function hoặc method khác.
-Delegate là sgfObject, tức là nó được reference counted, nên dùng sgfPtr thay cho pointer thường.
-Để đơn giản hoá và chỉ dùng để xử lý event. Delegate trong SGF chỉ có thể là một method hoặc function có dạng:

void functionA(SEventStruct& arg);
hoặc
class A
{
void methodA(SEventStruct& arg);
}

Tức là ko return gì. Chấp nhận một tham số dạng reference. SEventStruct thường là một structure chứa thông tin về event.

Event: là một sự kiện như bấm chuột, nút, bắt đầu game, kết thúc game, mở đầu level...
-Một event có thể được xử lý bởi nhiều Delegate. Event và delegate muốn kết hợp, phải có chung kiểu SEventStruct. Tức là một delegate viết để xử lý sự kiện bấm phím
thì phải chấp nhận kiểu SKeyboardEvent, nếu xử lý chuột thì phải chấp nhận SMouseEvent.
-Đăng ký delegate bằng addDelegate, bỏ đăng ký bằng removeDelegate.
-Việc bỏ đăng ký rất quan trọng, cần được làm khi object bị xoá(trong onRemove hoặc destructor)
-Các event phổ biến:
-KeyboardEvent
-MouseEvent(cả 2 đều thuộc inputManager)
-FrameStart&FrameEndEvent (bắt đầu vào đầu hoặc cuối một frame, thuộc core)
-GUIEvent(thuộc core)

Entity: đơn vị xây dựng nên game. Có thể là những thứ hữu hình như nhân vật, quái vật. Hoặc vô hình như trigger. Giống như sceneNode trong irrlicht
BaseClass của chúng là sgfEntity.
Entity sau khi tạo cần được đưa vào entityManager bằng addEntity.
Tương tự, muốn xoá entity cần dùng removeEntity.
Mọi initialization và deinitialization có ảnh hưởng đến game nên được thực hiện trong 2 method: onAdd và onRemove vì lúc đó entity mới nhận được con trỏ tới
manager của mình.
Khi một entity muốn được update liên tục, nó cần gọi manager->setActive(this,true);
Khi đó, method update của nó được tự động gọi từng frame.

Level: Là các trạng thái khác nhau của game. Có hai method onEnter và onExit. Level cần được load qua entityManager với loadLevel();
Menu, splash screen , in-game ... đều được coi là các level.
Khi một level mới được load, onLevelEnd được gọi trên tất cả các entity cũ. Những entity có thuộc tính persistent=false sẽ bị xoá. Chỉ có những entity như
nhân vật(persistent=true) là có thể tồn tại qua mọi level. Sau đó onLevelStart được gọi.

sgfIrrLevel(chưa hoàn thiện): là một class level đặc biệt. Nó có khả năng load một .irr scene của irrlicht. Sau đó nó sẽ dò các sceneNode và tạo ra các C++ object tương ứng từ
tên của sceneNode đó. Để điều này có thể xảy ra, cần registerClass(...) trên tất cả các class sẽ được tạo từ việc load level.
Các class đó cần có một static method có dạng như sau: sgfEntity**createFromNode(ISceneNode** node);
Tham khảo cách mà character và terrain được tạo từ .irr. sceneNode đó chỉ mang tính chất tham khảo về vị trí, rotation ,scale ... Nó nên được remove()
sau khi sử dụng xong. Trong demo của SGF, bạn có thể dùng một cái hộp, đặt tên nó là Character mà demo vẫn chạy bình thường.

DynamicVar, globalVars: biến toàn cục biến động. Dù biết sử dụng nó là xấu, nhưng đôi khi cũng cần. Trong core, có cung cấp một cái gọi là globalVars. Thực
chất đó là môi trường global của script. Các biến trong đó đều động, ko có kiểu, ko cần khai báo. Có thể sử dụng để trao đổi thông tin giữa các thành phần.
Đây cũng là cách để đưa object từ C++ vào script. Trong Terrain.h, terrain sử dụng nó để đưa triangle selector cho character.
Trong Demo3.cpp, guiSkin đã được đưa vào script, việc setSkin từ đó được thực hiện qua script.

ScriptVM: máy ảo của script. Có thể chạy script từ một file nén thông qua hệ thống file ảo của irrlicht. Có thể biên dịch script thành bytecode để tăng tốc độ
load(chứ ko phải execute). Có thể giúp script sử dụng các class đến từ C++. Sẽ được dùng để viết các hành động mang tính kịch bản trong game. Việc
bind c++ vào script được viết rõ trong file pdf đính kèm. Việc đưa các object từ C++ vào script hoặc lấy C++ object ra từ trong script được thể hiện trong
Demo3.cpp .