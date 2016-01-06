#include <Stick/StickString.hpp>
#include <Stick/StickDynamicArray.hpp>
#include <Stick/StickRBTree.hpp>
#include <Stick/StickMap.hpp>
#include <Stick/StickHashMap.hpp>
#include <Stick/StickError.hpp>
#include <Stick/StickThread.hpp>
#include <Stick/StickConditionVariable.hpp>
#include <Stick/StickHighResolutionClock.hpp>
#include <Stick/StickSystemClock.hpp>
#include <Stick/StickTest.hpp>
#include <Stick/StickStringConversion.hpp>
#include <Stick/StickPath.hpp>
#include <limits>

using namespace stick;

/*
void printNode(const char * _prefix, RBTree<Int32> & _tree, RBTree<Int32>::Node * _n, Size _depth)
{
    const char * col = _n->color == RBTree<Int32>::Color::Red ? "red" : "black";
    std::cout<<_prefix<<" depth: "<<_depth++<<" VAL: "<<_n->value<<" Col: "<<col<<std::endl;

    if(_n->left)
    {
        printNode("LFT", _tree, _n->left, _depth);
    }
    if(_n->right)
    {
        printNode("RGT: ", _tree, _n->right, _depth);
    }
}*/

class SomeClass
{
public:

    SomeClass()
    {
        std::cout << "SomeClass()" << std::endl;
    }

    ~SomeClass()
    {
        std::cout << "~SomeClass()" << std::endl;
    }
};

struct DestructorTester
{
    static int destructionCount;

    ~DestructorTester()
    {
        ++destructionCount;
    }

    static void reset()
    {
        destructionCount = 0;
    }
};

int DestructorTester::destructionCount = 0;


const Suite spec[] =
{
    SUITE("String Tests")
    {
        String a("test");
        String b("test");
        String c(String("different"));
        EXPECT(a == b);
        EXPECT(a != c);
        EXPECT(b != c);

        char expectedResults[] = {'t', 'e', 's', 't'};

        Int32 i = 0;
        for (auto c : a)
        {
            EXPECT(c == expectedResults[i]);
            i++;
        }

        i = 3;
        for (auto it = a.rbegin(); it != a.rend(); it++)
        {
            EXPECT(*it == expectedResults[i]);
            i--;
        }

        String d;
        d = a;
        EXPECT(d == a);
        EXPECT(d == "test");

        d = "another one";
        EXPECT(d == "another one");

        String e;
        EXPECT(e != d);

        String f;
        f.reserve(20);
        EXPECT(f.capacity() == 20);
        EXPECT(f.length() == 0);

        String ff("bla");
        ff.reserve(10);
        EXPECT(ff[0] == 'b');
        EXPECT(ff[1] == 'l');
        EXPECT(ff[2] == 'a');
        EXPECT(ff.length() == 3);

        String g;
        g.resize(10);
        EXPECT(g.length() == 10);

        String h("abcde");
        EXPECT(h[0] == 'a');
        EXPECT(h[1] == 'b');
        EXPECT(h[2] == 'c');
        EXPECT(h[3] == 'd');
        EXPECT(h[4] == 'e');
        h.resize(7, 'f');
        EXPECT(h.length() == 7);
        EXPECT(h[0] == 'a');
        EXPECT(h[1] == 'b');
        EXPECT(h[2] == 'c');
        EXPECT(h[3] == 'd');
        EXPECT(h[4] == 'e');
        EXPECT(h[5] == 'f');
        EXPECT(h[6] == 'f');

        {
            //test append function
            String i;
            String j(" World!");
            i.append("Hello");
            i.append(j);
            EXPECT(i.length() == 12);
            EXPECT(i[0] == 'H');
            EXPECT(i[1] == 'e');
            EXPECT(i[2] == 'l');
            EXPECT(i[3] == 'l');
            EXPECT(i[4] == 'o');
            EXPECT(i[5] == ' ');
            EXPECT(i[6] == 'W');
            EXPECT(i[7] == 'o');
            EXPECT(i[8] == 'r');
            EXPECT(i[9] == 'l');
            EXPECT(i[10] == 'd');
            EXPECT(i[11] == '!');

            i.append('c');
            EXPECT(i.length() == 13);
            EXPECT(i[12] == 'c');

            String k("fml");
            String l(" not");
            k.append(" test", l, 'c');
            EXPECT(k.length() == 13);
            EXPECT(k[0] == 'f');
            EXPECT(k[1] == 'm');
            EXPECT(k[2] == 'l');
            EXPECT(k[3] == ' ');
            EXPECT(k[4] == 't');
            EXPECT(k[5] == 'e');
            EXPECT(k[6] == 's');
            EXPECT(k[7] == 't');
            EXPECT(k[8] == ' ');
            EXPECT(k[9] == 'n');
            EXPECT(k[10] == 'o');
            EXPECT(k[11] == 't');
            EXPECT(k[12] == 'c');

            //test variadic constructor
            String m = String::concat("abc", l, 'c');
            EXPECT(m.length() == 8);
            EXPECT(m[0] == 'a');
            EXPECT(m[1] == 'b');
            EXPECT(m[2] == 'c');
            EXPECT(m[3] == ' ');
            EXPECT(m[4] == 'n');
            EXPECT(m[5] == 'o');
            EXPECT(m[6] == 't');
            EXPECT(m[7] == 'c');

            String n(12);
            EXPECT(n.capacity() == 12);
        }
        {
            //find tests
            String a("Hello World!");
            Size i = a.findIndex('o');
            EXPECT(i == 4);
            i = a.findIndex('o', 6);
            EXPECT(i == 7);
            i = a.rfindIndex('o');
            EXPECT(i == 7);
            i = a.rfindIndex('o', 5);
            EXPECT(i == 4);
            i = a.rfindIndex('z');
            EXPECT(i == String::InvalidIndex);
            i = a.findIndex('z');
            EXPECT(i == String::InvalidIndex);
        }
        {
            //substring tests
            String a("What's Up!");
            String b = a.sub(2, 2);
            EXPECT(b == "at");
            String c = a.sub(7);
            EXPECT(c.length() == 3);
            EXPECT(c == "Up!");
            std::cout<<"DA C: "<<c.cString()<<std::endl;
        }

        //copy tests
        {
            String a("blubb");
            String b(a);
            EXPECT(a == b);
            b = "fart";
            EXPECT(b == "fart");
            String c("weee");
            b = c;
            EXPECT(b == c);

            String mc(move(b));
            EXPECT(mc == c);

            mc = move(c);
            EXPECT(mc == "weee");
        }
    },
    SUITE("String Conversion Tests")
    {
        String s = toString(Int32(99));
        EXPECT(s == "99");
        EXPECT(toString(UInt32(12)) == "12");
        EXPECT(toString(Int64(10248295198789512)) == "10248295198789512");
        EXPECT(toString(UInt64(9128589127875)) == "9128589127875");
        EXPECT(toString(Size(79485763)) == "79485763");
        EXPECT(toHexString(847, 6, false, true) == "0x00034f");
        EXPECT(toHexString(Int64(847), 6, true, false) == "00034F");
        EXPECT(toHexString(-847, 0, true, false) == "-34F");
        EXPECT(toHexString(Size(1245135), 0, true, false) == "12FFCF");
        EXPECT(toHexString(UInt32(3), 0, true, true) == "0x3");
        EXPECT(toHexString(UInt16(9), 0, true, true) == "0x9");
        EXPECT(toHexString(UInt8(5), 0, true, true) == "0x5");
        EXPECT(toHexString(Int32(3), 0, true, true) == "0x3");
        EXPECT(toHexString(Int16(-9), 0, true, true) == "-0x9");
        EXPECT(toHexString(Int8(5), 0, true, true) == "0x5");

        EXPECT(toInt32("-3") == -3);
        EXPECT(toUInt32("99993") == 99993);
        EXPECT(toInt64("1234") == 1234);
        EXPECT(toInt64("-1234") == -1234);
        EXPECT(toInt16("-255") == -255);
    },
    SUITE("DynamicArray Tests")
    {
        DynamicArray<Int32> a;
        EXPECT(a.count() == 0);
        EXPECT(a.byteCount() == 0);

        a.resize(5);
        EXPECT(a.count() == 5);
        EXPECT(a.byteCount() == 20);

        for (Int32 i = 0; i < 5; ++i)
        {
            a[i] = i;
        }

        EXPECT(a[0] == 0);
        EXPECT(a[1] == 1);
        EXPECT(a[2] == 2);
        EXPECT(a[3] == 3);
        EXPECT(a[4] == 4);

        a.append(10);
        EXPECT(a.count() == 6);

        EXPECT(a[5] == 10);
        EXPECT(a.back() == 10);
        EXPECT(a.front() == 0);

        a.removeBack();
        EXPECT(a.back() == 4);
        EXPECT(a.count() == 5);

        Int32 expectedResults[] = {0, 1, 2, 3, 4};
        Int32 i = 0;
        for (auto e : a)
        {
            EXPECT(e == expectedResults[i]);
            ++i;
        }

        i = a.count() - 1;
        for (auto it = a.rbegin(); it != a.rend(); ++it)
        {
            EXPECT(*it == expectedResults[i]);
            --i;
        }

        a.clear();
        EXPECT(a.count() == 0);

        DynamicArray<Int32> b;
        Int32 arr[] = {1, 2, 3, 4};
        b.append(10);
        b.append(11);
        b.append(12);
        b.append(13);
        auto it = b.insert(b.end(), arr, arr + 4);
        EXPECT(it == b.end() - 4);
        EXPECT(it == b.begin() + 4);
        EXPECT(*it == 1);
        auto it2 = b.insert(b.begin() + 2, 99);
        EXPECT(it2 == b.begin() + 2);
        EXPECT(*it2 == 99);

        Int32 expectedArr[] = {10, 11, 99, 12, 13, 1, 2, 3, 4};
        i = 0;
        for (auto v : b)
        {
            EXPECT(expectedArr[i] == v);
            i++;
        }
        EXPECT(b.count() == 9);

        auto it3 = b.remove(b.begin() + 2, b.begin() + 6);
        EXPECT(it3 == b.begin() + 2);
        EXPECT(*it3 == 2);
        Int32 expectedArr2[] = {10, 11, 2, 3, 4};
        i = 0;
        for (auto v : b)
        {
            EXPECT(expectedArr2[i] == v);
            i++;
        }

        //destructor tests
        DynamicArray<DestructorTester> tt;
        tt.append(DestructorTester());
        tt.append(DestructorTester());
        tt.append(DestructorTester());
        tt.append(DestructorTester());
        tt.append(DestructorTester());
        DestructorTester::reset();
        tt.remove(tt.begin(), tt.begin() + 2);
        EXPECT(DestructorTester::destructionCount == 2);
        tt.clear();
        EXPECT(DestructorTester::destructionCount == 5);

        DynamicArray<Int32> ttt({1, 2, 3, 4, 5});
        EXPECT(ttt.count() == 5);
        EXPECT(ttt[0] == 1);
        EXPECT(ttt[1] == 2);
        EXPECT(ttt[2] == 3);
        EXPECT(ttt[3] == 4);
        EXPECT(ttt[4] == 5);

        DynamicArray<Int32> copy = ttt;
        EXPECT(copy.count() == 5);
        EXPECT(copy[0] == 1);
        EXPECT(copy[1] == 2);
        EXPECT(copy[2] == 3);
        EXPECT(copy[3] == 4);
        EXPECT(copy[4] == 5);

        DynamicArray<Int32> tttt = {0,52,1,3};
        copy = tttt;
        EXPECT(copy.count() == 4);
        EXPECT(copy[0] == 0);
        EXPECT(copy[1] == 52);
        EXPECT(copy[2] == 1);
        EXPECT(copy[3] == 3);

        DynamicArray<Int32> mcopy = move(copy);
        EXPECT(mcopy.count() == 4);
        EXPECT(mcopy[0] == 0);
        EXPECT(mcopy[1] == 52);
        EXPECT(mcopy[2] == 1);
        EXPECT(mcopy[3] == 3);

        mcopy = move(ttt);
        EXPECT(mcopy.count() == 5);
        EXPECT(mcopy[0] == 1);
        EXPECT(mcopy[1] == 2);
        EXPECT(mcopy[2] == 3);
        EXPECT(mcopy[3] == 4);
        EXPECT(mcopy[4] == 5);
    },
    SUITE("Path tests")
    {   
        String path = "/Absolute/Path/";
        EXPECT(path::isAbsolute(path));
        EXPECT(!path::isRelative(path));

        StringArray pathSegments = path::segments(path);
        EXPECT(pathSegments.count() == 2);
        EXPECT(pathSegments[0] == "Absolute");
        EXPECT(pathSegments[1] == "Path");

        String filePath = "../foo.txt";
        EXPECT(path::isRelative(filePath));

        path::SplitResult sp = path::split(filePath);

        EXPECT(sp.left == "..");
        EXPECT(sp.right == "foo.txt");
        EXPECT(path::directoryName(filePath) == "..");
        EXPECT(path::fileName(filePath) == "foo.txt");
        EXPECT(path::extension(filePath) == ".txt");

        path::SplitResult extsp = path::splitExtension(filePath);

        EXPECT(extsp.left == "../foo");
        EXPECT(extsp.right == ".txt");

        String jp = path::join("/Foo", "Bar");
        EXPECT(jp == "/Foo/Bar");

        StringArray segs;
        segs.append("Foo");
        segs.append("Bar");
        segs.append("Tar");
        
        String fs = path::fromSegments(segs);

        EXPECT(fs == "Foo/Bar/Tar");

        String fs2 = path::fromSegments(segs, true, true);

        EXPECT(fs2 == "/Foo/Bar/Tar/");

        //Path normalization
        String doubleSlashPath = "/Foo//Bar";
        EXPECT(path::normalize(doubleSlashPath) == "/Foo/Bar");

        String weirdPath = "Foo/./Bar/../bazz.jpg";
        EXPECT(path::normalize(weirdPath) == "Foo/bazz.jpg");
        String weirdPathTwo = "../Bar/./bazz.jpg";
        EXPECT(path::normalize(weirdPathTwo) == "Bar/bazz.jpg");
        EXPECT(path::normalize(weirdPathTwo, false) == "../Bar/bazz.jpg");

        //directory name
        String dirdirdir("/foo/bar/bazz");
        String dirdir = path::directoryName(dirdirdir);
        EXPECT(dirdir == "/foo/bar");
        String dir = path::directoryName(dirdir);
        EXPECT(dir == "/foo");
    },
    SUITE("RBTree Tests")
    {
        RBTree<Int32> emptyTree;

        RBTree<Int32> tree;
        EXPECT(tree.count() == 0);

        tree.insert(6);

        EXPECT(tree.root()->value == 6);

        tree.insert(5);
        tree.insert(7);
        tree.insert(9);
        tree.insert(8);
        tree.insert(1);
        tree.insert(24);
        tree.insert(2);
        tree.insert(3);

        EXPECT(tree.count() == 9);

        auto n = tree.find(5);
        auto n2 = tree.find(2);
        auto n3 = tree.find(24);

        EXPECT(n != nullptr);
        EXPECT(n2 != nullptr);
        EXPECT(n3 != nullptr);

        EXPECT(n->value == 5);
        EXPECT(n2->value == 2);
        EXPECT(n3->value == 24);

        auto notFound = tree.find(199);
        EXPECT(notFound == nullptr);

        tree.remove(1);
        tree.remove(8);
        tree.remove(24);
        EXPECT(tree.count() == 6);

        auto n4 = tree.find(1);
        auto n5 = tree.find(8);
        auto n6 = tree.find(24);

        EXPECT(n4 == nullptr);
        EXPECT(n5 == nullptr);
        EXPECT(n6 == nullptr);

        //copy construct
        RBTree<String> bla;
        bla.insert("yes");
        bla.insert("no");
        bla.insert("awesome");

        auto tree2 = bla;
        auto n21 = tree2.find("yes");
        auto n22 = tree2.find("no");
        auto n23 = tree2.find("awesome");

        EXPECT(n21 != nullptr);
        EXPECT(n22 != nullptr);
        EXPECT(n23 != nullptr);
        EXPECT(n21->value == "yes");
        EXPECT(n22->value == "no");
        EXPECT(n23->value == "awesome");

        auto tree3 = move(tree2);
        auto n212 = tree3.find("yes");
        auto n222 = tree3.find("no");
        auto n232 = tree3.find("awesome");

        //after moving, the pointers from tree2 should now sit in tree3
        EXPECT(n21 == n212);
        EXPECT(n22 == n222);
        EXPECT(n23 == n232);

        RBTree<String> bla2;
        bla2.insert("hello");
        bla2.insert("world");

        RBTree<String> bla2Copy;
        bla2Copy = bla2;
        auto n31 = bla2Copy.find("hello");
        auto n32 = bla2Copy.find("world");
        EXPECT(n31 != nullptr);
        EXPECT(n32 != nullptr);
        EXPECT(n31->value == "hello");
        EXPECT(n32->value == "world");

        RBTree<String> bla3Copy;
        bla3Copy = move(bla2Copy);
        auto n312 = bla3Copy.find("hello");
        auto n322 = bla3Copy.find("world");
        EXPECT(n312 == n31);
        EXPECT(n322 == n32);
    },
    SUITE("Map Tests")
    {
        typedef Map<String, Int32> TestMapType;
        TestMapType map;
        auto res = map.insert("a", 1);

        EXPECT(res.inserted == true);
        EXPECT((*res.iterator).value == 1);
        EXPECT(res.iterator->value == 1);
        EXPECT(res.iterator->key == "a");
        EXPECT(map.count() == 1);

        auto res2 = map.insert("a", 2);
        EXPECT(res2.inserted == false);
        EXPECT(res2.iterator->value == 2);
        EXPECT(map.count() == 1);

        map.insert("b", 3);
        map.insert("c", 4);
        EXPECT(map.count() == 3);

        auto it = map.find("b");
        EXPECT(it != map.end());
        EXPECT(it->value == 3);

        it = map.find("notThere");
        EXPECT(it == map.end());

        map.insert("d", 5);
        map.insert("e", 6);

        map["e"] = 7;
        map["f"] = 8;
        it = map.find("e");
        EXPECT(it->value == 7);
        EXPECT(map["f"] == 8);
        EXPECT(map.count() == 6);

        it = map.begin();

        auto lastKey = it->key;
        auto lastVal = it->value;

        it++;
        EXPECT(it->key != lastKey);
        EXPECT(it->value != lastVal);
        --it;
        EXPECT(it->key == lastKey);
        EXPECT(it->value == lastVal);


        TestMapType::KeyType * expectedKeys = new TestMapType::KeyType[map.count()];
        TestMapType::ValueType * expectedVals = new TestMapType::ValueType[map.count()];

        Size i = 0;
        for (const auto & kv : map)
        {
            expectedKeys[i] = kv.key;
            expectedVals[i] = kv.value;
            i++;
        }

        EXPECT(i == map.count());

        --i;
        Size j = 0;
        for (auto rit = map.rbegin(); rit != map.rend(); ++rit)
        {
            EXPECT(expectedKeys[i] == rit->key);
            EXPECT(expectedVals[i] == rit->value);
            ++j;
            --i;
        }

        EXPECT(j == map.count());

        delete [] expectedVals;
        delete [] expectedKeys;

        //remove tests
        it = map.find("f");
        auto it3 = it + 1;
        auto it2 = map.remove(it);
        EXPECT(it3 == it2);
        EXPECT(map.count() == 5);

        it = map.find("c");
        it++;
        it2 = map.remove("c");
        EXPECT(it == it2);
        EXPECT(map.count() == 4);

        Map<Int32, String> amap = {{3, "test"}, {2, "bla"}, {199, "blubb"}};
        EXPECT(amap.count() == 3);
        auto tit = amap.find(3);
        EXPECT(tit->key == 3);
        EXPECT(tit->value == "test");
        tit = amap.find(2);
        EXPECT(tit->key == 2);
        EXPECT(tit->value == "bla");
        tit = amap.find(199);
        EXPECT(tit->key == 199);
        EXPECT(tit->value == "blubb");

        //copy tests
        Map<String, Int32> mapToCopy;
        mapToCopy.insert("hello", 1);
        mapToCopy.insert("world", 2);
        auto copy = mapToCopy;
        EXPECT(copy.count() == 2);
        EXPECT(copy["hello"] == 1);
        EXPECT(copy["world"] == 2);

        auto copy2 = move(copy);
        EXPECT(copy2.count() == 2);
        EXPECT(copy2["hello"] == 1);
        EXPECT(copy2["world"] == 2);

        Map<String, Int32> mapToCopy2;
        mapToCopy2.insert("a", 12);
        mapToCopy2.insert("b", 21);
        mapToCopy2.insert("c", 33);

        copy2 = mapToCopy2;
        EXPECT(copy2.count() == 3);
        EXPECT(copy2["a"] == 12);
        EXPECT(copy2["b"] == 21);
        EXPECT(copy2["c"] == 33);

        Map<String, Int32> mapToCopy3;
        mapToCopy3.insert("arr", 5);
        mapToCopy3.insert("gh", 6);

        copy2 = move(mapToCopy3);
        EXPECT(copy2.count() == 2);
        EXPECT(copy2["arr"] == 5);
        EXPECT(copy2["gh"] == 6);
    },
    SUITE("HashMap Tests")
    {
        HashMap<String, Int32> hm(1);

        hm.insert("test", 1);
        hm.insert("test", 2);
        EXPECT(hm.count() == 1);
        hm.insert("anotherKey", 3);
        auto res = hm.insert("blubb", 4);
        EXPECT(hm.count() == 3);
        EXPECT(res.iterator->key == "blubb");
        EXPECT(res.iterator->value == 4);
        EXPECT(res.inserted == true);
        res = hm.insert("blubb", 5);
        EXPECT(res.iterator->value == 5);
        EXPECT(res.inserted == false);
        auto it = hm.remove("anotherKey");
        EXPECT(hm.count() == 2);

        auto it2 = hm.find("blubb");
        EXPECT(it2->key == "blubb");
        EXPECT(it2->value == 5);
        
        //copy tests
        auto cpy = hm;
        EXPECT(cpy.count() == 2);
        EXPECT(cpy["blubb"] == 5);
        EXPECT(cpy["test"] == 2);

        HashMap<String, Int32> copyMe;
        copyMe["a"] = 1;
        copyMe["b"] = 2;
        copyMe["c"] = 3;
        auto cpied = move(copyMe);
        EXPECT(cpied.count() == 3);
        EXPECT(cpied["a"] == 1);
        EXPECT(cpied["b"] == 2);
        EXPECT(cpied["c"] == 3);

        cpied = cpy;
        EXPECT(cpied.count() == 2);
        EXPECT(cpied["blubb"] == 5);
        EXPECT(cpied["test"] == 2);

        HashMap<String, Int32> copyMe2;
        copyMe2["d"] = 199;
        cpy = move(copyMe2);
        EXPECT(cpy.count() == 1);
        EXPECT(cpy["d"] == 199);
    },
    SUITE("Thread Tests")
    {
        Thread thread;
        Error err = thread.join();
        EXPECT(err.code() != 0);
        std::atomic<bool> bJoinable(false);
        std::atomic<bool> bValidThreadID(false);
        err = thread.run([&]() { bValidThreadID = thread.threadID() != 0; bJoinable = thread.isJoinable(); });
        EXPECT(err.code() == 0);
        err = thread.join();
        EXPECT(bJoinable);
        EXPECT(bValidThreadID);
        EXPECT(err.code() == 0);
        EXPECT(!thread.isJoinable());
    },
    SUITE("ConditionVariable Tests")
    {
        Thread thread;
        ConditionVariable cond;
        Mutex m;
        Error err = thread.run([&]() { ScopedLock<Mutex> lock(m); cond.wait(lock); });
        auto ss = SystemClock::now();
        auto start = HighResolutionClock::now();
        thread.sleepFor(Duration::fromSeconds(0.05));
        std::cout<<(HighResolutionClock::now() - start).seconds()<<std::endl;
        std::cout<<(SystemClock::now() - ss).seconds()<<std::endl;
        EXPECT(err.code() == 0);
        EXPECT(thread.isJoinable() == true); //the thread should still run as its blocking on the condition var
        err = cond.notifyOne();
        EXPECT(err.code() == 0);
        err = thread.join();
        EXPECT(err.code() == 0);
    }
};

int main(int _argc, const char * _args[])
{
    return runTests(spec, _argc, _args);
}
