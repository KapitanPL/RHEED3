#include "globals.h"

//qt globals
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTime>

//logger
#include <filesystem>
#include <ios>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>


#ifdef _DEBUG
    #include <QDebug>
#endif

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32




C_globals::C_globals()
{
	m_sGlobalPath = QDir::currentPath();
}

C_globals & C_globals::getInstance()
{
    static C_globals m; //this constructs this
    return m;
}

C_globals::~C_globals()
{}

void C_globals::Load()
{
    if (QDir().exists(m_sConfigPath))
    {
        QString name = QString("guiData");
        if (QFile::exists(QString("%1/%2.json").arg(m_sConfigPath).arg(name)))
        {
            QFile fGuiData(QString("%1/%2.json").arg(m_sConfigPath).arg(name));
            if (fGuiData.open(QIODevice::ReadOnly))
            {
                 QByteArray saveData = fGuiData.readAll();

                 QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

                 QJsonObject loadVariant = loadDoc.object();
                 if (loadVariant.contains("m_mainWindowSize") && loadVariant["m_mainWindowSize"].isObject())
                 {
                     QJsonObject size = loadVariant["m_mainWindowSize"].toObject();
                     if (size.contains("width"))
                         m_mainWindowSize.setWidth(size["width"].toInt());
                     if (size.contains("height"))
                         m_mainWindowSize.setHeight(size["height"].toInt());
                 }
                 if (loadVariant.contains("m_mainWindowPos") && loadVariant["m_mainWindowPos"].isObject())
                 {
                     QJsonObject size = loadVariant["m_mainWindowPos"].toObject();
                     if (size.contains("x"))
                         m_mainWindowPos.setX(size["x"].toInt());
                     if (size.contains("y"))
                         m_mainWindowPos.setY(size["y"].toInt());
                 }
                 if (loadVariant.contains("m_sLogPath") && loadVariant["m_sLogPath"].isString())
                 {
                     m_sLogPath = loadVariant["m_sLogPath"].toString();
                     while (m_sLogPath.endsWith("/") || m_sLogPath.endsWith("\\"))
                     {
                        m_sLogPath.chop(1);
                     }
                 }
                 if (loadVariant.contains("m_iLogLevel"))
                 {
                     m_iLogLevel = loadVariant["m_iLogLevel"].toInt();
                 }
                 if (loadVariant.contains("m_listDebugEntries") && loadVariant["m_listDebugEntries"].isArray())
                 {
                     QJsonArray entries = loadVariant["m_listDebugEntries"].toArray();
                     for (auto entry : entries)
                     {
                         m_listDebugEntries.append(entry.toString());
                     }
                 }
             }
         }
     }
}

void C_globals::Save()
{
    if (!QDir().exists(m_sConfigPath))
    {
        QDir().mkdir(m_sConfigPath);
    }
    QString name = QString("guiData");
    QFile fGuiData(QString("%1/%2.json").arg(m_sConfigPath).arg(name));
    if (fGuiData.open(QIODevice::WriteOnly))
    {
        QJsonObject saveObject;
        //save here
        QJsonObject size;
        size["width"] = QJsonValue(m_mainWindowSize.width());
        size["height"] = QJsonValue(m_mainWindowSize.height());
        saveObject["m_mainWindowSize"] = size;

        QJsonObject position;
        position["x"] = QJsonValue(m_mainWindowPos.x());
        position["y"] = QJsonValue(m_mainWindowPos.y());
        saveObject["m_mainWindowPos"] = position;

        QJsonValue sLogPath = QJsonValue(m_sLogPath);
        saveObject["m_sLogPath"] = sLogPath;

        QJsonValue iLogLevel = QJsonValue(m_iLogLevel);
        saveObject["m_iLogLevel"] = iLogLevel;

        QJsonArray listLogSymbols;
        for (auto itSymbol : m_listDebugEntries)
        {
            listLogSymbols.append(QJsonValue(itSymbol));
        }
        saveObject["m_listDebugEntries"] = listLogSymbols;

        QJsonDocument saveDoc(saveObject);
        fGuiData.write(saveDoc.toJson());

        fGuiData.close();
    }
}

void C_globals::GetCameraPluginPath(QString& sPath)
{
	QString sLoacalPath = m_sGlobalPath;
	QString sCamera = m_sCameraPluginPath;
	if (sCamera.contains("/"))
		sCamera = sCamera.right(sCamera.length() - sCamera.lastIndexOf("/")-1);
	if (sCamera.contains("\\"))
		sCamera = sCamera.right(sCamera.length() - sCamera.lastIndexOf("\\")-1);
	if (!sLoacalPath.endsWith("/"))
		sLoacalPath.append("/");
	sLoacalPath.append(sCamera);
	sPath = sLoacalPath;
}

void C_globals::GetCameraPluginPath(std::wstring& Path)
{
	QString sqPath;
	GetCameraPluginPath(sqPath);
	Path = sqPath.toStdWString();
}

void C_globals::setupLogger()
{
    m_pLogger = &C_logger::getInstance();
    if (m_pLogger)
    {
        QTime now = QTime::currentTime();
        QDate today = QDate::currentDate();
        QString sLogPath = QString("%0/%1/%2/%3").arg(m_sLogPath).arg(today.year()).arg(today.month()).arg(today.day());

        if (!QDir().exists(sLogPath))
        {
            QDir().mkpath(sLogPath);
        }
        QString sFileName = QString("%0/%1_%2").arg(sLogPath,"Rheed3_1",now.toString("hh_mm_ss"));
        int iIncrementor = 0;
        while (true)
        {
            QString sTMPFileName = iIncrementor == 0 ? QString("%0.log").arg(sFileName) : QString("%0_%1.log").arg(sFileName, iIncrementor);
            if (QFile::exists(sTMPFileName))
            {
                iIncrementor++;
            }
            else
            {
                sFileName = sTMPFileName;
                break;
            }
        }
        if (!m_pLogger->init(sFileName.toStdWString()))
            m_pLogger = nullptr;
        if (m_pLogger)
        {
            m_pLogger->logMsg(L"RHEED3 program started, initiating log.");
        }

    }
}

C_logger::~C_logger()
{
#ifdef _DEBUG
    qDebug() << "~C_logger";
#endif
#ifdef _DEBUG
    qDebug() << "~C_logger: First";
#endif
    m_condVar.notify_one(); //starts saving the first queue
    swapQueues();
#ifdef _DEBUG
    qDebug() << "~C_logger: Second";
#endif
    m_condVar.notify_one(); //save the second queue;
    m_bRun = false;
    {
        std::unique_lock<std::mutex> lck(m_mutexSwap);
    }
    m_condVar.notify_one(); //save the second queue;
    if (m_pworkThread && m_pworkThread->joinable())
    {
#ifdef _DEBUG
    qDebug() << "~C_logger: waiting for join";
#endif
        m_pworkThread->join();
        delete m_pworkThread;
#ifdef _DEBUG
    qDebug() << "~C_logger: joined";
#endif
    }
#ifdef _DEBUG
    qDebug() << "~C_logger: closing File";
#endif
    m_file.close();
}

C_logger& C_logger::getInstance()
{
    static C_logger S;
    return S;
}

void C_logger::work(C_logger *pThis)
{
    do
    {
        if (pThis->m_file.good())
        {
            std::unique_lock<std::mutex> queueLock(pThis->m_mutexSwap);
#ifdef _DEBUG
    qDebug() << "~work: locked && " << pThis->m_storeQueue.empty();
#endif
            while (!pThis->m_storeQueue.empty())
            {
                pThis->m_file << pThis->m_storeQueue.front().c_str() << "\n";
                pThis->m_storeQueue.pop();
            }
        }
        std::unique_lock<std::mutex> lck(pThis->m_mutexRun);
        pThis->m_condVar.wait(lck);
    }while (pThis->m_bRun);
}

bool C_logger::init(std::wstring sFileName)
{
	if (!std::filesystem::exists(sFileName))
	{
		m_sFileName = sFileName;
		m_file.open(m_sFileName, std::ios::in | std::ios::app);
		if (m_file.good())
		{
			m_pworkThread = new std::thread(work, this);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}

void C_logger::logMsg(std::wstring sMsg)
{
    if (sMsg.empty() || !m_bRun)
        return;
    //std::string sCompleteMsg;
    std::wstringstream sMsgStream;
	std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *tNow = std::localtime(&tt);
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    double dTime = std::chrono::duration<double>(now).count();
    sMsgStream << "<TIME>";
    int years = int(floor(dTime/31556926));
    sMsgStream << tNow->tm_year << "_";
    dTime -= double(years)*31556926;
    int months = int(floor(dTime/2629743.83));
    sMsgStream << tNow->tm_mon << "_";
    dTime -= months*2629743.83;
    int days = int(floor(dTime/86400));
    sMsgStream << tNow->tm_mday << " ";
    dTime -= (double)days*86400;
    int hours = int(floor(dTime/3600));
    dTime -= (double)hours*3600;
    int mins = int(floor(dTime/60));
    dTime -= (double)mins*60;
    sMsgStream<< tNow->tm_hour << ":" << tNow->tm_min << ":" << dTime;
    sMsgStream<< "<\\TIME>";

    sMsgStream << "<THREAD>" << std::this_thread::get_id() << "<\\THREAD>";
    sMsgStream << "<MSG>" << sMsg << "<\\MSG>";
    sMsg = sMsgStream.str();
    { //scope only for lock
        std::unique_lock<std::mutex> queueLock(m_mutexSwap);
        m_bufferQueue.emplace(sMsg);
    }
    swapQueues();
    m_condVar.notify_one(); //run the storing thread
}

std::wstring C_logger::getFileName()
{
    if (m_file.good())
        return m_sFileName;
    else
        return L"";
}

uint32_t C_logger::getLineCount()
{
    if (m_file.good())
        return m_uiLines;
    else
        return 0;
}

C_logger::C_logger()
{
}

void C_logger::swapQueues()
{
    std::unique_lock<std::mutex> queueLock(m_mutexSwap);
    if (m_storeQueue.empty())
        std::swap(m_storeQueue, m_bufferQueue);
#ifdef _DEBUG
    qDebug() << "swap";
#endif
}

C_LibLoader::C_LibLoader(std::wstring sLibName, I_PMS_V01* host, S_Version minVersion)
{
#ifdef _WIN32
	typedef I_PMS_V01* (__cdecl* GetInterface)();

	HMODULE lib = LoadLibrary(sLibName.c_str());
	if (lib)
	{
		GetInterface getPluginInterface = (GetInterface)GetProcAddress(lib, "GetInterface");
		if (getPluginInterface && getPluginInterface() != nullptr)
		{
			I_PMS_V01* pPlugin = getPluginInterface();
			if (pPlugin)
			{
				if (pPlugin->Attach(host, host->sVersion, minVersion) == eOK)
				{  
					m_bValid = true;
					m_lib = lib;
					m_servant = pPlugin;
					m_master = host;
				}
			}
		}
		if (!IsValid())
			FreeLibrary(lib);
	}
#endif // _WIN32
}

C_LibLoader::~C_LibLoader()
{
#ifdef _WIN32
	if (m_bValid && m_lib && m_servant)
	{
		m_servant->Detach(m_master);
		FreeLibrary(static_cast<HMODULE>(m_lib));
	}
#endif // _WIN32

}

bool C_LibLoader::IsValid()
{
	return m_bValid;
}

I_PMS_V01* C_LibLoader::GetInterface()
{
	return m_servant;
}
