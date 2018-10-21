#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class SubjectInterface;
class ObservableInterface;

class ObserverInterface {
public:
    void SetObverableInstance(ObservableInterface* o) { obv = o; }
    virtual void OnUpdateSubject(SubjectInterface* sub) = 0;
#define SUBJECT(id) (*obv)[(id)]
protected:
    ObservableInterface* obv;
};

class SubjectInterface {
public:
    SubjectInterface() {}
    SubjectInterface(std::string _id) : id(_id) {}
    std::string GetId() const { return id; }
    void SetId(std::string _id) { id = _id; }

    void AddObserver(ObserverInterface* ob) {
        obs.push_back(ob);
    }

    void RemoveObserver(ObserverInterface* ob) {
        std::vector<ObserverInterface*>::iterator it = std::find(obs.begin(), obs.end(), ob);
        if (it != obs.end()) {
            obs.erase(it);
        }
    }
  
    void Update(const SubjectInterface& sub) {
        if (DoUpdate(&sub)) {
            NotifyToObserver();
        }
    }
    void UpdateForce(const SubjectInterface& sub) {
        DoUpdate(&sub);
        NotifyToObserver();
    }



protected:
    std::vector<ObserverInterface*> obs;
    std::string id;

    void NotifyToObserver() {
        for (auto it : obs) {
            if (it) {
                it->OnUpdateSubject(this);
            }
        }
    }
    virtual bool DoUpdate(const SubjectInterface* sub) { return true; }
};

template <typename T>
class SimpleSubjectInterface : public SubjectInterface {
public:
    SimpleSubjectInterface(std::string _id, T v) : value(v) {
        id = _id;
    }  
    SimpleSubjectInterface() { }
    T GetValue() const { return value; }
protected:
    T value;

    virtual bool DoUpdate(const SubjectInterface* sub) {
        if (!sub) return false;
        SimpleSubjectInterface* ss = (SimpleSubjectInterface*)sub;
        if (ss->GetValue() == value) return false;
        value = ss->GetValue();
        return true;
    }
};


class ObservableInterface {
public:
    template <typename T>
    void AddSimpleSubject(T v) {

    }

    template <typename SubjectType>
    void AddSubject(SubjectType* sub) {
        if (sub) {
            subjects[sub->GetId()] = sub;
        }
    }

    void RemoveSubject(std::string id) {
        std::map<std::string, SubjectInterface*>::iterator it = subjects.find(id);
        if (it != subjects.end()) {
            delete it->second;
            subjects.erase(it);
        }
    }

    void AddObserver(std::string id, ObserverInterface* ob) {
        std::map<std::string, SubjectInterface*>::iterator it = subjects.find(id);
        if (it != subjects.end()) {
            ob->SetObverableInstance(this);
            it->second->AddObserver(ob);
        }
    }

    void RemoveObserver(std::string id, ObserverInterface* ob) {
        std::map<std::string, SubjectInterface*>::iterator it = subjects.find(id);
        if (it != subjects.end()) {
            it->second->RemoveObserver(ob);
        }
    }

    SubjectInterface*& operator[](std::string id) {
        std::map<std::string, SubjectInterface*>::iterator it = subjects.find(id);
        if (it != subjects.end()) {
            return it->second;
        }
        else {
            subjects[id] = nullptr;
            return subjects[id];
        }
    }
        
protected:
    std::map<std::string, SubjectInterface*> subjects;
};



class StringSubject : public SimpleSubjectInterface<std::string> {
public:
    StringSubject(std::string _id, std::string _value) {
        id = _id;
        value = _value;
    }

};

class NullSubject : public SubjectInterface {
public:
    NullSubject() {}
    NullSubject(std::string _id) {
        id = _id;
    }
};


// FOR WTL
#define ON_WND_STATE_START(_winstate_id) if(_winstate_id) { WndStateHandled = false;
#define ON_WND_STATE_END() }
#define ON_WND_STATE(id, func) if(!WndStateHandled && id == sub->GetId()) { WndStateHandled = true; func(); }
class MyWindow : public ObserverInterface {
public:
    
    void OnUpdateWindowTitle() {
        cout << "update title" << endl;
        SetWndStateHandled(false);
    }
    // 必须提供该接口，尽量避免直接调用，能走消息队列尽量走消息队列
    void OnUpdateSubject(SubjectInterface* sub) {
        StringSubject* ss = (StringSubject*)sub;
        cout << "receive subject, id: " << sub->GetId()
            << " value:" << ss->GetValue()  << endl;

        ON_WND_STATE_START(1)
            ON_WND_STATE("window_title", OnUpdateWindowTitle)
            ON_WND_STATE("window_title", OnUpdateWindowTitle)
            ON_WND_STATE("page_num", OnUpdateWindowTitle)
        ON_WND_STATE_END()

        SUBJECT("window_title_notify")->Update(StringSubject("", "nima"));
    }

protected:
    bool WndStateHandled;
    void SetWndStateHandled(bool handled) {
        WndStateHandled = handled;
    }
};


class RemoteService : public ObserverInterface {
public:
    void OnUpdateWindowTitle() {
        cout << "remote service detect title change..." << endl;
    }

    void OnUpdateSubject(SubjectInterface* sub) {
        if (sub->GetId() == "notify_abc") {
            cout << "receive abc" << endl;
            return;
        }
        StringSubject* ss = (StringSubject*)sub;
        OnUpdateWindowTitle();
    }
};


// 是否需要考虑消息的发送和接收方，因为都可能会发送接收消息，避免不必要的消息推送
// 当前可以定义两个不同的消息规避
int main() {
    ObservableInterface obv;

    obv.AddSubject<StringSubject>(new StringSubject("window_title", "title"));
    obv.AddSubject<StringSubject>(new StringSubject("window_title_notify", "title"));
    obv.AddSubject<NullSubject>(new NullSubject("notify_abc"));

    MyWindow* win = new MyWindow();
    obv.AddObserver("window_title", win);

    RemoteService* rmt = new RemoteService();
    obv.AddObserver("window_title_notify", rmt);
    obv.AddObserver("notify_abc", rmt);
    
    if (obv["window_title"]) {
        obv["window_title"]->Update(StringSubject("window_title", "窗口新名称"));

        // 强制更新
        obv["window_title_notify"]->UpdateForce(StringSubject("", "nima"));
    }
    obv["notify_abc"]->Update(NullSubject());
    return 0;
}