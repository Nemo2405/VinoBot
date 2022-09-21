/*
 * DbDispatcher.cpp
 *
 *  Created on: Aug 16, 2022
 *      Author: eddyneshton
 */


#include "DbDispatcher.h"
using namespace pqxx;

// in how many years is equivalent to 1 degree
const double AGE_COEFF = 10.0;

template<typename T>
T getDataFromUserDb(connection * conn, std::string field, int chatId) {
	pqxx::work W(*conn);
	pqxx::row r = W.exec1("SELECT " + field + " FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null()) {
	 	return -1;
	}
	return r[0].as<T>();
}

template<>
std::string getDataFromUserDb<std::string>(pqxx::connection *conn, std::string field, int chatId) {
	pqxx::work W(*conn);
	pqxx::row r = W.exec1("SELECT " + field + " FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null() || r[0].as<std::string>() == "") {
	 	return "";
	}
	return r[0].as<std::string>();
}

DbDispatcher::DbDispatcher() :
	dbConn(nullptr) {
    ConfigReader reader;
    ConfigReader::DbConfig temp = reader.getConnectionData();
    std::cout << "DataBase connection data: " <<
                 "\nDataBase Name: "  <<temp.DbName   << " " <<
                 "\nUserName: "       <<temp.UserName << " " <<
                 "\nPassword: "<<temp.UserPassword << " " <<
                 "\nHost Address: "   <<temp.HostName << " " <<
                 "\nPort: "           <<temp.Port     << std::endl << std::endl;

	geoDecoder = new GeoDecoder();
	matcher = new Matcher();


	try {
        dbConn = new pqxx::connection("dbname = "   + std::string(temp.DbName) +
                                      " user = "    + std::string(temp.UserName) +
                                      " password = "+ std::string(temp.UserPassword) +
                                      " hostaddr = "+ std::string(temp.HostName) +
                                      " port = "    + std::to_string(temp.Port));
		if (dbConn->is_open()) {
			std::cout<<"Db succesfully opened\n";
		} else {
			std::cout<<"error opening Db\n";
			dbConn = nullptr;
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		dbConn = nullptr;
	}

}

DbDispatcher::~DbDispatcher() {
	if (dbConn!=nullptr) {
		dbConn->disconnect();
		std::cout<<"closed connection";
	}
}

void DbDispatcher::execQuery(std::string query) {
	// create transaction
	pqxx::work W(*dbConn);
	W.exec( query );
	W.commit();
}


void DbDispatcher::addUser(int chatId, std::string username) {
	execQuery("INSERT INTO userdata (chat_id, current_state, username) VALUES (" + std::to_string(chatId) + ", "+ std::to_string(IND_E_AGE_EDIT) +", '"+ username + "');");
	std::cout<<"new user " << chatId << " was added\n";
}


int DbDispatcher::getAge(int chatId) {
	return getDataFromUserDb<int>(dbConn, "age", chatId);
}

void DbDispatcher::setAge(int chatId, int newAge) {
	execQuery("UPDATE userdata SET age = " + std::to_string(newAge) + " WHERE chat_id = " + std::to_string(chatId) + ";");
}


int DbDispatcher::getState(int chatId) {
	pqxx::work W(*dbConn);
	pqxx::row existanceRow = W.exec1("SELECT COUNT (*) FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (existanceRow[0].as<int>() == 0) {
	 	return IND_NO_AUTH;
	}
	return getDataFromUserDb<int>(dbConn, "current_state", chatId);
}

void DbDispatcher::setState(int chatId, int newState) {
	execQuery("UPDATE userdata SET current_state = " + std::to_string(newState) + " WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"state was updated to " << newState << std::endl;
}


void DbDispatcher::setGender(int chatId, int newGender) {
	execQuery("UPDATE userdata SET sex = " + std::to_string(newGender) + " WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"gender was updated to " << newGender << std::endl;
}

void DbDispatcher::setTargetGender(int chatId, int newTargetGender) {
	execQuery("UPDATE userdata SET target_sex = " + std::to_string(newTargetGender) + " WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"target gender was updated to " << newTargetGender << std::endl;
}


void DbDispatcher::setAim(int chatId, int isRelationships) {
	execQuery("UPDATE userdata SET aim = " + std::to_string(isRelationships) + " WHERE chat_id = " + std::to_string(chatId) + ";");
}

int DbDispatcher::getAim(int chatId) {
	return getDataFromUserDb<int>(dbConn, "aim", chatId);
}


std::string DbDispatcher::getName(int chatId) {
	return getDataFromUserDb<std::string>(dbConn, "name", chatId);
}

void DbDispatcher::setName(int chatId, std::string newName) {
	execQuery("UPDATE userdata SET name = '" + newName + "' WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"name was updated to " << newName << std::endl;
}


bool DbDispatcher::isDescriptionPresent (int chatId) {
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT description FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null() || r[0].as<std::string>() == "") {
	   	return false;
	}
	//std::cout <<  r[0].as<int>()<< std::endl;
	return true;
}

void DbDispatcher::setDescription (int chatId, std::string newDescription) {
	execQuery("UPDATE userdata SET description = '" + newDescription + "' WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"description was updated to " << newDescription << std::endl;
}


std::string DbDispatcher::getCity(int chatId) {
	return getDataFromUserDb<std::string>(dbConn, "city", chatId);
}

void DbDispatcher::setLocation(int chatId, double lat, double lon) {
	std::string res = std::async(std::launch::async, [&](){
		return geoDecoder->getCity(lat, lon);
	}).get();
	execQuery("UPDATE userdata SET city = '" + res + "',"
			" longitude = " + std::to_string(lon)+ ","
			" latitude = " + std::to_string(lat) + " WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout  << "---" << res << "---\n";

}


void DbDispatcher::setImage(int chatId, std::string imageId) {
	execQuery("UPDATE userdata SET image = '" + imageId + "' WHERE chat_id = " + std::to_string(chatId) + ";");
	//std::cout<<"writtem "<<imageId<<std::endl;
}

//std::pair<std::string, std::string> DbDispatcher::getUserDescription(int chatId) {
std::tuple<int, std::string, std::string> DbDispatcher::getUserDescription(int chatId) {
	std::string picture;
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT image FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null() || r[0].as<std::string>() == "") {
	   	picture = "";
	} else 	{
		picture = r[0].as<std::string>();
	}
	//picture = getDataFromUserDb<std::string>(dbConn, "image", chatId);

	std::string caption;
	pqxx::work W1(*dbConn);
	pqxx::row r1 = W1.exec1("SELECT name, age, city, description FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W1.commit();

	//name
	if (r1[0].is_null() || r1[0].as<std::string>() == "") {
	   	caption = "";
	} else {
		caption = r1[0].as<std::string>();
	}
	caption += ", ";

	//age
	if (!r1[1].is_null()) {
		caption += r1[1].as<std::string>() + ", ";
	}

	//city
	if (!r1[2].is_null()) {
		caption += r1[2].as<std::string>();
	}

	//description
	if (!r1[3].is_null() && r1[3].as<std::string>() != "") {
		caption += " - " + r1[3].as<std::string>();
	}
	//std::cout<<"readen" << picture;

	return std::tuple<int, std::string, std::string>(chatId, picture, caption);
}

void DbDispatcher::addAdTick(int chatId) {
	// (a + freq) % freq
	execQuery("UPDATE userdata SET "
			"current_ad_tick = "
			"((SELECT current_ad_tick FROM userdata WHERE chat_id = " + std::to_string(chatId) + ") + 1 + "
			"(SELECT ad_freq FROM utilities)) % (SELECT ad_freq FROM utilities) "
			"WHERE chat_id = " + std::to_string(chatId) + ";");
}

bool DbDispatcher::isAdTick(int chatId) {
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT current_ad_tick FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null() || r[0].as<int>() == 0) {
		return true;
	}
	return false;
}

std::vector<int> splitAdString(std::string adSeq, std::string delimiter) {
	std::vector<int> res;

	size_t pos = 0;
	std::string token;
    try {
        while ((pos = adSeq.find(delimiter)) != std::string::npos) {
            token = adSeq.substr(0, pos);
            //std::cout << token << std::endl;
            res.push_back(std::stoi(token));
            adSeq.erase(0, pos + delimiter.length());
        }
        res.push_back(std::stoi(adSeq));
    }  catch (std::invalid_argument &e) {
        return std::vector<int>();
    }

	return res;
	//std::cout << s << std::endl;
}

std::string DbDispatcher::getAd (int chatId) {
	int currentAd;
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT current_ad_seq FROM userdata WHERE chat_id = " + std::to_string(chatId) + ";");
	W.commit();
	if (r[0].is_null())
		currentAd = 0;
	else
		currentAd = r[0].as<int>();

	execQuery("UPDATE userdata SET current_ad_seq = (" + std::to_string(currentAd + 1) + " % (SELECT total_ad FROM utilities)) WHERE chat_id = " + std::to_string(chatId) + ";");
	//UPDATE userdata SET current_ad_seq = (2 % (SELECT total_ad FROM utilities))
	pqxx::work W1(*dbConn);
	pqxx::row r1 = W1.exec1("SELECT ad_sequence FROM utilities;");
	W1.commit();
	std::string adSeq = r1[0].as<std::string>();

	pqxx::work W2(*dbConn);
	pqxx::row r2 = W2.exec1("SELECT content FROM advertising WHERE ad_id = " + std::to_string(splitAdString(adSeq, ":")[currentAd]) + ";");
	W2.commit();
	if (r2[0].is_null())
		return "";
	else
		return r2[0].as<std::string>();
}

void DbDispatcher::resetWatched (int chatId) {
	execQuery("DELETE FROM watched_list WHERE subject = " + std::to_string(chatId) + ";");
}

void DbDispatcher::addToWatched (int subjectChatId, int targetChatId) {
	execQuery("INSERT INTO watched_list VALUES (" + std::to_string(subjectChatId) + ", " + std::to_string(targetChatId) + ");");
}

int DbDispatcher::matchPartner (int userChatId) {
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT sex, aim, target_sex FROM userdata WHERE chat_id = " + std::to_string(userChatId) + ";");
	W.commit();

	if (r[0].is_null() || r[1].is_null() || r[2].is_null()) {
		return -1;
	}


	// get self blanc
	pqxx::work W1(*dbConn);
	pqxx::row r1 = W1.exec1("SELECT chat_id, latitude, longitude, age "
							"FROM userdata "
							"WHERE chat_id = " + std::to_string(userChatId) + ";");
	W1.commit();




	// find all users by sex
	pqxx::nontransaction N(*dbConn);

	std::string firQuery = "SELECT chat_id, latitude, longitude, age "
							"FROM userdata "
							"WHERE (";
	for (Matcher::GenderSpec nowGenderSpec :
			matcher->getTargetGenderSpec(Matcher::GenderSpec({r[0].as<int>(), r[1].as<int>(), r[2].as<int>()}))) {
		firQuery += "(sex = " 		+ std::to_string(nowGenderSpec.meFemale) 		+ " AND "
					"aim = "  		+ std::to_string(nowGenderSpec.aimRelationship) + " AND "
					"target_sex = " + std::to_string(nowGenderSpec.targetFemale) 	+ ") OR ";
	}

	firQuery.erase(firQuery.size() - 4, firQuery.size()); // remove last or
	firQuery += " ) "
			"AND NOT EXISTS "
			"(SELECT target FROM watched_list WHERE subject = " + std::to_string(userChatId) + ") " //exclude watcher
			"AND current_state <> 33;";	// exclude left profiles
	//std::cout<<firQuery<< std::endl;
	pqxx::result R = N.exec(firQuery);

	return matcher->getBestMatchId(MiniProfile(	r1[0].as<int>()	  ,
												r1[1].as<double>(),
												r1[2].as<double>(),
												r1[3].as<int>())  , MiniProfileWrapper(R));
}


bool DbDispatcher::validateAdminPwd(std::string target) {
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT control_pwd FROM utilities;");
	W.commit();
	if (r[0].is_null()) {
		if (target == "321") {
				return true;
			}
		return false;
	}

	if (r[0].as<std::string>() == target) {
		return true;
	}
	return false;
}

std::vector<int> DbDispatcher::getUsersChatIds() {
	std::vector<int> res;
	pqxx::nontransaction N(*dbConn);
	pqxx::result R = N.exec("SELECT chat_id FROM userdata;");

	for (pqxx::result::const_iterator nowRes = R.begin(); nowRes != R.end(); ++nowRes) {
		res.push_back(nowRes[0].as<int>());
	}

	return res;
}

void DbDispatcher::deleteBlockerUser(int chatId) {
	execQuery("DELETE FROM userdata WHERE chat_id="+std::to_string(chatId));
	std::cout<<"deleted user " << chatId << " as blocker..." <<std::endl;
}

void DbDispatcher::addLike(int subjectChatId, int targetChatId) {
	execQuery("INSERT INTO favourites (subject, target) VALUES (" 	+ std::to_string(subjectChatId) + ", "
																	+ std::to_string(targetChatId) + ");");
}

int DbDispatcher::getLikerCount(int targetChatId) {
	pqxx::work W(*dbConn);
	pqxx::row r = W.exec1("SELECT COUNT (subject) FROM favourites WHERE target = " + std::to_string(targetChatId) + " ;");
	W.commit();
	if (r[0].is_null()) {
		return 0;
	}
	return r[0].as<int>();
}

std::vector<int> DbDispatcher::getLikers(int targetChatId) {
	std::vector<int> res;
	pqxx::nontransaction N(*dbConn);
	pqxx::result R = N.exec("SELECT subject FROM favourites WHERE target = " + std::to_string(targetChatId) + ";");

	for (pqxx::result::const_iterator nowRes = R.begin(); nowRes != R.end(); ++nowRes) {
		res.push_back(nowRes[0].as<int>());
	}
	return res;
}

void DbDispatcher::removeLiker(int subjectChatId, int targetChatId) {
	execQuery("DELETE FROM favourites WHERE subject = " + std::to_string(subjectChatId) +
									  " AND target = " + std::to_string(targetChatId)+ ";");
}

std::string DbDispatcher::getUname(int chatId) {
	return getDataFromUserDb<std::string>(dbConn, "username", chatId);
}

//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// by latitude and longitude seperatly
MiniProfile::MiniProfile (int chat, double latit, double longit, int age)  :
	m_chatId(chat),
	m_location(Location({latit, longit})),
	m_age(age) {}

//by whole location
MiniProfile::MiniProfile (int chat, Location locat, int age) :
	m_chatId(chat),
	m_location(locat),
	m_age(age) {}

int_fast16_t MiniProfile::getChatId() const {
	return m_chatId;
}

double MiniProfile::latitude() const {
	return m_location.latitude;
}

double MiniProfile::longitude() const {
	return m_location.longitude;
}

double MiniProfile::age() const {
	return m_age;
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MiniProfileWrapper::MiniProfileWrapper(pqxx::result R) {
	for (pqxx::result::const_iterator nowRes = R.begin(); nowRes != R.end(); ++nowRes) {
//		std::cout << nowRes[0].as<int>() <<" " <<
//					 nowRes[1].as<double>()<<" " <<
//					 nowRes[2].as<double>()<<" " <<
//					 nowRes[3].as<int>()<<std::endl;
		m_profiles.push_back(MiniProfile(nowRes[0].as<int>(),
										 nowRes[1].as<double>(),
										 nowRes[2].as<double>(),
										 nowRes[3].as<int>()));
	}
}

void MiniProfileWrapper::excludeMe(int myId) {
	int_fast16_t index = -1;
	for (int_fast16_t iter = 0; iter < m_profiles.size(); iter ++ ) {
		if (m_profiles[iter].getChatId() == myId) {
			index = iter;
			//std::cout<<index<<" FOUND!\n";
			break;
		}
	}
	if (index != -1) {
		//std::cout<<index<<" DELETING!\n";
		m_profiles.erase(m_profiles.begin() + index);
	}
}

int_fast16_t MiniProfileWrapper::size() const {
	return m_profiles.size();
}

double range3d (const MiniProfile &_1, const MiniProfile &_2) {
	return std::sqrt(std::pow(_1.latitude() - _2.latitude(), 2) +
					 std::pow(_1.longitude() - _2.longitude(), 2) +
					 std::pow((_1.age() - _2.age()) / AGE_COEFF, 2));  ////
}

void MiniProfileWrapper::sortProfiles(MiniProfile zero) {
	std::sort(m_profiles.begin(), m_profiles.end(), [zero](const MiniProfile &_1, const MiniProfile &_2) {
		if (range3d(_1, zero) < range3d(_2, zero)) return true;
		return false;
	});
}

int_fast16_t MiniProfileWrapper::getBestId() {
	return m_profiles[0].getChatId();
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int_fast16_t Matcher::getBestMatchId(MiniProfile me, MiniProfileWrapper userList) {

	userList.excludeMe(me.getChatId());
	//std::cout << "Size 1: " << userList.size() << std::endl;
	if (userList.size() < 1) {
			return -2;	// no people
	}
	userList.sortProfiles(me);
	return userList.getBestId();
}


std::vector<Matcher::GenderSpec> Matcher::getTargetGenderSpec(GenderSpec self) {
	if (self.meFemale == 0) {
		if (self.aimRelationship == 0) {
			switch (self.targetFemale) {
			case 0: {
				return std::vector<GenderSpec>({{0,0,0}, {0,0,2}});
				break;
			}
			case 1: {
				return std::vector<GenderSpec>({{1,0,0}, {1,0,2}});
				break;
			}
			case 2: {
				return std::vector<GenderSpec>({{0,0,0}, {0,0,1}, {0,0,2},
												{1,0,0}, {1,0,1}, {1,0,2}});
				break;
			}
			default: {
				return std::vector<GenderSpec>({{0,0,0}, {0,0,1}, {0,0,2},
												{1,0,0}, {1,0,1}, {1,0,2}});
			}
			}
		} else {
			switch (self.targetFemale) {
			case 0: {
				return std::vector<GenderSpec>({{0,1,0}, {0,1,2}});
				break;
			}
			case 1: {
				return std::vector<GenderSpec>({{1,1,0}, {1,1,2}});
				break;
			}
			case 2: {
				return std::vector<GenderSpec>({{0,1,0}, {0,1,2},
												{1,1,0}, {1,1,2}});
				break;
			}
			default: {
				return std::vector<GenderSpec>({{0,1,0}, {0,1,2},
												{1,1,0}, {1,1,2}});
			}
			}
		}
	} else {
		if (self.aimRelationship == 0) {
			switch (self.targetFemale) {
			case 0: {
				return std::vector<GenderSpec>({{0,0,1}, {0,0,2}});
				break;
			}
			case 1: {
				return std::vector<GenderSpec>({{1,0,1}, {1,0,2}});
				break;
			}
			case 2: {
				return std::vector<GenderSpec>({{0,0,0}, {0,0,1}, {0,0,2},
												{1,0,0}, {1,0,1}, {1,0,2}});
				break;
			}
			default: {
				return std::vector<GenderSpec>({{0,0,0}, {0,0,1}, {0,0,2},
												{1,0,0}, {1,0,1}, {1,0,2}});
			}
			}
		} else {
			switch (self.targetFemale) {
			case 0: {
				return std::vector<GenderSpec>({{0,1,1}, {0,1,2}});
				break;
			}
			case 1: {
				return std::vector<GenderSpec>({{1,1,1}, {1,1,2}});
				break;
			}
			case 2: {
				return std::vector<GenderSpec>({{0,1,1}, {0,1,2},
												{1,1,1}, {1,1,2}});
				break;
			}
			default: {
				return std::vector<GenderSpec>({{0,1,1}, {0,1,2},
												{1,1,1}, {1,1,2}});
			}
			}
		}
	}
}


ConfigReader::DbConfig ConfigReader::getConnectionData()
{
    try {
        std::string fileName = "dbConfig";
        std::ifstream fin(fileName);
        DbConfig res;
        //std::string line;
        fin.getline(res.DbName, 50);
        fin.getline(res.UserName, 50);
        fin.getline(res.UserPassword, 50);
        fin.getline(res.HostName, 50);
        char temp[50];
        fin.getline(temp, 50);
        try {
            res.Port = std::stoi(temp);
        } catch (std::invalid_argument &e) {
            std::cout << "error port " << std::endl;

            res.Port = 5432;
        }

        return res;


    }  catch (std::exception &e) {
        std::cout << "Error while reading dbConfig file" << std::endl;
        throw;
    }

}
