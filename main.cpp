#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <cstdlib>
using wt=std::string;//word type
class book
{
    public:
    book(wt name,wt author);
    book()=default;
    ~book()=default;

    book& operator=(book b);
    const wt getName() const;
    const wt getAuthor() const;
    const wt getRenter() const;
    const bool isRented() const;
    void setName(wt);
    void setAuthor(wt);
    void setRent(wt);
    void unRent();
    private:
    bool rented;
    wt name,author,renter;
};
book::book(wt name,wt author):name(name),author(author) {rented=false;renter="";}
const wt book::getName() const {return this->name;}
const wt book::getAuthor() const {return this->author;}
const wt book::getRenter() const {return this->renter;}
const bool book::isRented() const {return this->rented;}
void book::setName(wt name) {this->name=name;}
void book::setAuthor(wt author) {this->author=author;}
void book::setRent(wt renter) {this->renter=renter;this->rented=1;}
void book::unRent() {this->renter="";this->rented=0;}
book& book::operator=(book b) 
{
    this->name=b.getName();
    this->author=b.getAuthor();
    return *this;
}

class library
{
    public:
    library();
    ~library()=default;

    void add(const book);
    void del(const int);
    const int findByName(const wt) const;
    const int findByRenter(const wt) const;
    book& operator[](const int);
    const int getSize() const;
    const std::vector<book> getStory() const;
    const bool rent(const int,const wt);
    private:
    std::vector<book> story;
};
library::library() {}
void library::add(const book b)  {story.push_back(b);}
void library::del(int id) {story.erase(story.begin()+id);}
const int library::findByName(wt name) const 
{
    for (int i=0;i<story.size();i++) 
        if (story[i].getName()==name) return i;
    return -1;
}
const int library::findByRenter(wt renter) const 
{
    for (int i=0;i<story.size();i++) 
        if (story[i].getRenter()==renter) return i;
    return -1;
}
const std::vector<book> library::getStory() const {return this->story;}
const bool library::rent(int id,wt renter) {this->story[id].setRent(renter);}
book& library::operator[](int index) {return *((this->story.begin())+index);}
const int library::getSize() const {return story.size();}
class console
{
    public:
    template<typename T> static void log(const T logtext,bool nl=true) {std::cout<<logtext<<(nl?'\n':'\0');}
    template<typename T> static void log(std::initializer_list<T> logtext,bool nl=true) {for (T obj:logtext) log(obj,nl);}
    template<typename T> static void err(const T logtext) {log(logtext);waitkey();}
    template<typename T> static void get(T& obj) {std::cin>>obj;}
    template<typename T> static const T get()
    {
        T obj;
        get(obj);
        return obj;
    }
    static void clear() {system("cls");}
    static void waitkey() {getchar();getchar();}
};

int main()
{
    library tsg;
    while(1)
    {
        console::clear();
        console::log({"a.ADD","c.CHANGE","d.DELETE","e.EXIT","f.FIND","h.HELP","l.LIST","r.RENT"});
        char choose;
        console::get<char>(choose);
        //[可选] <必须>
        switch (choose)
        {
            case 'a':
            {
                //添加书籍 批量添加使用{name author ...}
                console::clear();
                console::log("[({\\n)|(.\\0)]<name>[\\0|,|.]<author>[(\\n})|(\\0)]");
                console::log("添加书籍,单个添加使用.开头后跟上书名+作者\n批量添加在首尾单行使用{}包住内容,其他与前者相同");
                char c;
                console::get(c);
                wt bookName=console::get<wt>(),bookAuthor=console::get<wt>();
                if (c=='{')
                {
                    tsg.add({bookName,bookAuthor});
                    while (1)
                    {
                        bookName=console::get<wt>();
                        if (bookName=="}") break;
                        bookAuthor=console::get<wt>();
                        if (bookAuthor=="}") break;
                        //console::log({bookName,bookAuthor});
                        tsg.add({bookName,bookAuthor});
                    }
                }
                else if (c=='.')
                    tsg.add({bookName,bookAuthor});
                else 
                {
                    console::err("book addition failed");
                    console::waitkey();
                    break;
                }
                console::log("book addition succeed");
                console::waitkey();
                break;
            }
            case 'c':
            {
                console::clear();
                console::log("<id> <newName|-> <newAuthor|-> <newRenter|->");
                console::log("修改书的信息,输入id+新名字+新作者+新租赁者(后三者任一无需更改的话使用-占位)");
                int id=console::get<int>();
                if (id>=tsg.getSize()||id<0)
                {
                    console::err("id overflowed");
                    console::get<wt>();console::get<wt>();
                    break;
                }
                wt newName=console::get<wt>(),newAuthor=console::get<wt>(),newRenter=console::get<wt>();
                if (newName!="-") tsg[id].setName(newName);
                if (newAuthor!="-") tsg[id].setAuthor(newAuthor);
                if (newRenter!="-") tsg[id].setRent(newRenter);
                console::log("changed");
                console::waitkey();
                break;
            }
            case 'd':
            {
                console::clear();
                console::log("<id>");
                int id=console::get<int>();
                if (id>=tsg.getSize()||id<0)
                {
                    console::err("id overflowed");
                    break;
                }
                tsg.del(id);
                console::log("delete succeed");
                console::waitkey();
                break;
            }
            case 'e':
                return 0;
            case 'f':
            {
                console::clear();
                console::log("<name|renter>");
                wt bookFind=console::get<wt>();
                int idByName=tsg.findByName(bookFind),idByRenter=tsg.findByRenter(bookFind);
                if (idByName==-1&&idByRenter==-1) console::err("cannot found");
                else 
                {
                    if (idByName!=-1) console::log("find by book name:"+idByName);
                    if (idByRenter!=-1) console::log("find by book renter:"+idByRenter);
                }
                console::waitkey();
                break;
            }
            case 'h':
            {
                console::clear();
                console::log("帮助文本中+代表任意分隔符,./\\<>?;:'\"|[]均可");
                console::log("<>内是必须参数,[]代表可选参数,()代表内部是一个整体,|代表多种并列选项");
                console::waitkey();
                break;
            }
            case 'l':
            {
                console::clear();
                std::vector<book> story=tsg.getStory();
                if (story.empty()) 
                {
                    console::err("the story is empty");
                    console::waitkey();
                    break;
                }
                int count=0;
                for (const auto obj:story)
                {
                    console::log(count++,0);
                    console::log<wt>({" ",obj.getName()," ",obj.getAuthor()," ",obj.getRenter()},false);
                    console::log<char>('\0');
                }
                console::waitkey();
                break;
            }
            case 'r':
            {
                console::clear();
                console::log("<rent|unrent> <id> <renter>");
                wt choose=console::get<wt>();
                if (choose=="rent")
                {
                    int id=console::get<int>();
                    if (id<0||id>=tsg.getSize()) 
                    {
                        console::err("id overflow");
                        break;
                    }
                    book& rentBook=tsg[id];
                    if (rentBook.isRented()) 
                    {
                        console::err("already rented");
                        break;
                    }
                    wt renter=console::get<wt>();
                    rentBook.setRent(renter);
                    console::log<wt>({"book ",rentBook.getName()," written by ",rentBook.getAuthor(),"has already rented to ",rentBook.getRenter()},false);
                    console::waitkey();
                    break;
                }
                else if (choose=="unrent")
                {
                    int id=console::get<int>();
                    if (id<0||id>=tsg.getSize()) 
                    {
                        console::err("id overflow");
                        break;
                    }
                    book& rentBook=tsg[id];
                    if (rentBook.isRented()) 
                    {
                        rentBook.unRent();
                        break;
                    }
                    else
                    {
                        console::err("this book isn't rented");
                        break;
                    }
                }

            }
        }
    }
    
}