/*
 * AlgorithmRegistration.cpp
 *
 *  Created on: May 24, 2018
 *      Author: krimolovsky
 */

#include "AlgorithmRegistration.h"
#include "RPS_TournamentManager.h"


AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
	RPS_TournamentManager::getTournamentManager().registerAlgorithm(id, factoryMethod);
}
