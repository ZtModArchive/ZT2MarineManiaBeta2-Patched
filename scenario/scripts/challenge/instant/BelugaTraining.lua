-- BelugaTraining.lua
 
include "scenario/scripts/ui.lua";
include "scenario/scripts/Shows.lua";
include "scenario/scripts/entity.lua";
include "scenario/scripts/economy.lua";
include "scenario/scripts/misc.lua";

CHALLENGE_NAME = "belugatraining";
CHALLENGE_REWARD = 2500
function validate()

	if (isChalDeclinedForever(CHALLENGE_NAME) == true) then
		return 0;
	end
	
	-- Set the current challenge in a global variable for debugging purposes
	setglobalvar("current_challenge_name", CHALLENGE_NAME);
	
	local mgr = queryObject("BFScenarioMgr");
	if (mgr) then
		-- If we are gathering prereqs, output the challenge name
		if (getPrereqGather() == true) then
			BFLOG(SYSTRACE, CHALLENGE_NAME);
		end
				
		-- Increment the number of challenges which have been offered
		incChallengesOffered();
			
		-- Only give the scenario if we are not suppressing them
		if (getAddScenarioSuppression() == false) then
			mgr:BFS_ADDSCENARIO("scenario/goals/challenge/belugatraining.xml");
		elseif (getPrereqGather() == false) then
			logDebugChallengeInfo(CHALLENGE_NAME, "skipped");
		end
				
		return 1;
	end
		
	return 0;
end


function evalbelugatraining(comp)
	challenge = getglobalvar("challenge")
	if (challenge == "accept") then
		local mgr = queryObject("BFScenarioMgr");	
		if (mgr) then
			mgr:BFS_SHOWRULE(CHALLENGE_NAME);
		end
		
		logDebugChallengeInfo(CHALLENGE_NAME, "accepted");
		
		setchallengeactive();
		showprimarygoals();
		comp.accept = 1;
		
	elseif (challenge == "decline") then
		
		if (getDeclineForever() == true) then
			setglobalvar(CHALLENGE_NAME.."_decline_forever", "true");
			
			logDebugChallengeInfo(CHALLENGE_NAME, "declinedforever");
			
			-- If we make it here we want to reset decline forever
			setDeclineForever(false);
		elseif (getChallengeDebugMode() == true) then
			logDebugChallengeInfo(CHALLENGE_NAME, "declined");
		end

		return -1;
	end
	

	if (comp.accept == nil) then
		if (challenge == nil) then
			showinstantchallengepanel("Challengetext:CHBelugaTraining");
			setglobalvar("challenge", "waiting")
		end
	end

	if (comp.accept == 1) then
		if(hasSpeciesWithTrickOfLevelX("Beluga_Adult","BelugaJump",60)) then
			return 1;
		end
		-- 2 month time limit
		return checkTimeLimit(comp,2,0)
	end
	
	return 0;

end

function completebelugatraining(comp)
	
	giveCash(CHALLENGE_REWARD);
	
	showchallengewin("Challengetext:CHBelugaTrainingSuccess");
	resetchallengeoverandcomplete(CHALLENGE_NAME);	
end


function failbelugatraining(comp)
	resetchallengeover(CHALLENGE_NAME);
end
