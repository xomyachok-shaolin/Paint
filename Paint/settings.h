/* 2018 ReVoL Primer Template */
/* settings.h */
#pragma once
#define MAX_IBUFF 32
#define APPNAME "PAINT"
class Settings {
private:
    char pPath[260];
    char V[MAX_IBUFF], D[MAX_IBUFF];
public:
    Settings() {
        pPath[0] = 0;
    }
    void PutPath(char* Path) {
        strcpy(pPath, Path);
    }
    void PutSetting(char* Value, char* KeyName) {
        if (strlen(pPath) == 0) return;
        WritePrivateProfileString(APPNAME, KeyName, Value, pPath);
    }
    void GetSetting(char* Value, int Size, char* KeyName, char* Default) {
        Value[0] = 0;
        if (strlen(pPath) == 0) return;
        GetPrivateProfileString(APPNAME, KeyName, Default, Value, Size, pPath);
    }
    void PutSettingLong(int Value, char* KeyName) {
        if (strlen(pPath) == 0) return;
        _ltoa(Value, V, 10);
        WritePrivateProfileString(APPNAME, KeyName, V, pPath);
    }
    int GetSettingLong(char* KeyName, int Default) {
        if (strlen(pPath) == 0) return 0;
        _ltoa(Default, D, 10);
        V[0] = 0;
        GetPrivateProfileString(APPNAME, KeyName, D, V, MAX_IBUFF, pPath);
        return atol(V);
    }
};
