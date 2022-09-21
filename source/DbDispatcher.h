/*
 * DbDispatcher.h
 *
 *  Created on: Aug 16, 2022
 *      Author: eddyneshton
 */

#ifndef DBDISPATCHER_H_
#define DBDISPATCHER_H_
#include <pqxx/pqxx>
#include <tuple>
#include <fstream>
#include "GeoDecoder.h"

class MiniProfile {
public:
	struct Location {
		double latitude;
		double longitude;
	};

	MiniProfile (int chat, double latit, double longit, int age);
	MiniProfile (int chat, Location locat, int age);

	int_fast16_t getChatId() const;
	double latitude() const;
	double longitude() const;
	double age() const;

private:
	int m_chatId;
	Location m_location;
	int m_age;
};



class MiniProfileWrapper {
public:
	MiniProfileWrapper(){}
	MiniProfileWrapper(pqxx::result R);
	~MiniProfileWrapper(){}

	void excludeMe(int myId);
	int_fast16_t size() const;
	void sortProfiles(MiniProfile zero);
	int_fast16_t getBestId();

private:
	std::vector<MiniProfile> m_profiles;
};

class Matcher {
public:
	Matcher(){}
	~Matcher(){}

	struct GenderSpec {
		int meFemale;
		int aimRelationship;
		int targetFemale;
	};

	std::vector<GenderSpec> getTargetGenderSpec(GenderSpec self);

	int_fast16_t getBestMatchId(MiniProfile me, MiniProfileWrapper userList);

};

using namespace pqxx;

class ConfigReader {
public:
	typedef struct {
        char DbName         [50];
        char UserName       [50];
        char UserPassword   [50];
        char HostName       [50];
        char Port           [50];

    } DbConfig;

    DbConfig getConnectionData();
};

class DbDispatcher {
public:

	static const int IND_NO_AUTH			= -7;
	static const int IND_FLOW_MENU 			= 0;
	static const int IND_MAIN_MENU 			= 1;
	static const int IND_MESSAGE_MENU 		= 2;
	static const int IND_BLANC_EDIT 		= 3;
	static const int IND_E_AGE_EDIT 		= 4;
	static const int IND_E_USER_SEX_EDIT 	= 5;
	static const int IND_E_TARGET_SEX_EDIT 	= 6;
	static const int IND_E_NAME_EDIT 		= 7;
	static const int IND_E_LOCATION_EDIT 	= 8;
	static const int IND_E_DESCRIPTION_EDIT	= 9;
	static const int IND_E_DESCRIPT_EDIT	= 10;
	static const int IND_E_IMAGE_EDIT 		= 11;
	static const int IND_E_IMG_EDIT 		= 12;
	static const int IND_E_FINAL_EDIT 		= 13;
	static const int IND_DISABLE_ACCOUNT 	= 14;

	static const int IND_GENDER_MALE		= 20;
	static const int IND_GENDER_FEMALE		= 21;

	static const int IND_TARGET_GENDER_MALE	= 22;
	static const int IND_TARGET_GENDER_FEMALE= 23;
	static const int IND_TARGET_GENDER_NOMATT= 24;

	static const int IND_SKIP_DESCRIPTION	= 25;
	static const int IND_SKIP_DESCRIPT		= 26;

	static const int IND_E_CONFIRMATION_LOCATION 	= 27;
	static const int IND_E_CONFIRM_LOCATION 		= 28;
	static const int IND_E_DECLINE_LOCATION 		= 29;

	static const int IND_E_AIM 			= 30;
	static const int IND_E_AIM_RELAT 	= 31;
	static const int IND_E_AIM_FRIEN 	= 32;

	static const int IND_LEFT_STATE 	= 33;
	static const int IND_INVITE			= 34;

	static const int IND_ENDED_BLANCS	= 35;
	static const int IND_RESET_WATCHED 	= 36;

	static const int IND_SHOW_LIKER 	= 37;
	static const int IND_TO_LIKERS	 	= 38;
	static const int IND_LIKERS 		= 39;

	static const int IND_DIRECT_MSG 	= 40;
	static const int IND_DIRECT_CANCEL 	= 41;

	static const int ADM_LOGIN 			= 101;
	static const int ADM_MAIN_MENU 		= 102;
	static const int ADM_COMMON_MESSAGE = 103;

	DbDispatcher();
	virtual ~DbDispatcher();

	void addUser(int chatId, std::string username);

	int getAge(int chatId);

	void setAge(int chatId, int newAge);

	int getState(int chatId);

	void setState(int chatId, int newState);

	void setGender(int chatId, int newGender);

	void setTargetGender(int chatId, int newTargetGender);

	void setAim(int chatId, int isRelationships);

	int getAim(int chatId);

	std::string getName(int chatId);

	void setName(int chatId, std::string newName);

	bool isDescriptionPresent (int chatId);

	void setDescription (int chatId, std::string newDescription);

	std::string getCity(int chatId);

	void setLocation(int chatId, double lat, double lon);

	void setImage(int chatId, std::string imageId);

	//std::pair<std::string, std::string> getUserDescription(int chatId);
	std::tuple<int, std::string, std::string> getUserDescription(int chatId);

	void addAdTick(int chatId);

	bool isAdTick(int chatId);

	std::string getAd (int chatId);

	void resetWatched (int chatId);

	void addToWatched (int subjectChatId, int targetChatId);


	int matchPartner (int userChatId);


	bool validateAdminPwd(std::string target);

	std::vector<int> getUsersChatIds();

	void deleteBlockerUser(int chatId);

	void addLike(int subjectChatId, int targetChatId);

	int getLikerCount(int targetChatId);

	std::vector<int> getLikers(int targetChatId);

	void removeLiker(int subjectChatId, int targetChatId);

	std::string getUname(int chatId);

private:
	connection *dbConn;

	void execQuery(std::string query);

	GeoDecoder *geoDecoder;

	Matcher *matcher;
};

#endif /* DBDISPATCHER_H_ */
