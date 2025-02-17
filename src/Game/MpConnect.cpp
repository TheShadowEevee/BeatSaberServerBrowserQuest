#include "Game/MpConnect.hpp"
#include "Game/MpModeSelection.hpp"
using ServerBrowser::Game::MpModeSelection;

namespace ServerBrowser::Game {

	void MpConnect::Join(HostedGameData game) {
		getLogger().debug("MpConnect::Join");
		/* MQE hasn't been updated in awhile, but this my be useful in the future
		// MQE Version check
		if (game.get_MpExVersion().has_value()) {
			getLogger().debug("MpConnect::Join MpEx Check");
			//std::string ourMQE_Version = "Undefined";
			std::string ourMQE_Version = (Modloader::getMods().find("multiquestensions") != Modloader::getMods().end()) ? "0.6.2" : "";


			//semver::version checkRange2{ ourMQE_Version };
			//checkRange2.minor++;
			//auto range = semver::range::detail::range(string_format(">=%s <%s", ourMQE_Version.c_str(), checkRange2.to_string().c_str()));
			if (game.get_MpExVersion()->to_string() != ourMQE_Version/*range.satisfies(game.get_MpExVersion().value(), true)*//*) {
				std::string ourMQE_VersionStr = (!ourMQE_Version.empty() ? ourMQE_Version : "Not installed");
				std::string theirMpExVersionStr = game.get_MpExVersion()->to_string();

				getLogger().warning("Blocking game join because of MultiQuestensions/MultiplayerExtensions version mismatch ours: %s, theirs: %s", 
					ourMQE_Version.c_str(), theirMpExVersionStr.c_str());

				std::string MQE_Error(string_format(
					"MultiQuestensions/MultiplayerExtensions version difference detected!\r\n"
					"Please ensure you and the host are both using a compatible version.\r\n"
					"\r\n"
					"Your version: %s\r\n"
					"Their version: %s",
					ourMQE_VersionStr.c_str(),
					theirMpExVersionStr.c_str()
				));
				
				MpModeSelection::PresentConnectionFailedError(
					"Incompatible game",
					MQE_Error,
					false
				);
				return;
			}
		}*/
		// Master server switching
		if (!game.get_MasterServerHost().has_value() || game.get_MasterServerHost()->ends_with(OFFICIAL_MASTER_SUFFIX)) {
			getLogger().error("MpConnect::Join Official Server, this should never run!!!");
			// Game is hosted on the player platform's official master server
			//if (_usingModdedServer || !_officialEndPoint) {
			//	 If we normally use a modded server (e.g. because BeatTogether is installed), we need to now force-connect to our official server
			//	 
			//	 //TODO: NOT IMPLEMENTED
			//	 
			//	switch (MpLocalPlayer.Platform)
			//	{
			//	case UserInfo.Platform.Oculus:
			//		SetMasterServerOverride(OFFICIAL_MASTER_OCULUS);
			//		break;
			//	case UserInfo.Platform.PS4:
			//		SetMasterServerOverride(OFFICIAL_MASTER_PS4);
			//		break;
			//	case UserInfo.Platform.Test:
			//		// hmmm
			//		//   ^ Don't think of anything stupid there
			//		SetMasterServerOverride(OFFICIAL_MASTER_TEST);
			//		break;
			//	default:
			//	case UserInfo.Platform.Steam:
			//	case null:
			//		SetMasterServerOverride(OFFICIAL_MASTER_STEAM);
			//		break;
			//	}
			//}
			//else
			//{
				 //Clearing the override should fall back to the correct official server
				ClearMasterServerOverride();
			//}
		}
		else
		{
			getLogger().debug("MpConnect::Join MasterServer is not offical, overrides will not work, hopefully this is BeatTogether Master");
			// Game is hosted on a custom master server, we need to override
			SetMasterServerOverride(game.get_MasterServerHost().value(), game.get_MasterServerPort() != 0 ? game.get_MasterServerPort() : DEFAULT_MASTER_PORT);
		}

		getLogger().debug("MpConnect::Join Trigger join");
		// Trigger the actual join via server code
		MpModeSelection::ConnectToHostedGame(game);
	}

#pragma region Master Server Management

	//std::pair<std::string, int> MpConnect::OverrideEndPoint;

	//std::pair<std::string, int> MpConnect::LastUsedMasterServer;

	SafePtr<DnsEndPoint> MpConnect::OverrideEndPoint;

	SafePtr<DnsEndPoint> MpConnect::LastUsedMasterServer;

	DnsEndPoint* const MpConnect::get_OverrideEndPoint() {
		//return DnsEndPoint::New_ctor(il2cpp_utils::newcsstr(OverrideEndPoint.first), OverrideEndPoint.second)
		if (OverrideEndPoint)
			return static_cast<DnsEndPoint*>(OverrideEndPoint);
		else return nullptr;
	}

	//DnsEndPoint* MpConnect::get_LastUsedMasterServer() {
	//	return DnsEndPoint::New_ctor(il2cpp_utils::newcsstr(LastUsedMasterServer.first), LastUsedMasterServer.second)
	//}
	//void MpConnect::set_OverrideEndPoint(DnsEndPoint* NewOverrideEndPoint) {

	//}
	//void MpConnect::set_LastUsedMasterServer(DnsEndPoint* LastUsedEndPoint) {

	//}

	bool MpConnect::get_ShouldDisableCertificateValidation() {
		// We should disable certificate validation (X509CertificateUtilityPatch) if we are overriding to unofficial masters
		return OverrideEndPoint && !OverrideEndPoint->hostName->EndsWith(il2cpp_utils::newcsstr(OFFICIAL_MASTER_SUFFIX));
	}

	void MpConnect::ReportCurrentMasterServerValue(DnsEndPoint* currentEndPoint) {
		bool isFirstReport = !LastUsedMasterServer;

		LastUsedMasterServer = currentEndPoint;

		if (OverrideEndPoint && csstrtostr(currentEndPoint->ToString()) == csstrtostr(OverrideEndPoint->ToString())/*currentEndPoint->Equals(OverrideEndPoint)*/)
		{
			// This is our own override, not useful information
			return;
		}

		auto hostName = currentEndPoint->hostName;

		if (hostName->EndsWith(il2cpp_utils::newcsstr(OFFICIAL_MASTER_SUFFIX)))
		{
			// This is the official / default master server (likely not using a server mod)
			//officialEndPoint = currentEndPoint;
			//usingModdedServer = false;

			if (isFirstReport)
			{
				getLogger().info("Default master server appears to be official: ");
			}

			return;
		}

		// This is neither our override nor an official server, which means another mod is doing this
		//moddedEndPoint = currentEndPoint;
		//usingModdedServer = true;

		if (isFirstReport)
		{
			getLogger().warning("Default master server appears to be modded: ");
		}
	}

	void MpConnect::SetMasterServerOverride(std::string hostName, int port) {
		if (!classof(DnsEndPoint*)->initialized) il2cpp_functions::Class_Init(classof(DnsEndPoint*)); // This is needed in order to initialize the Il2CppClass
		//SetMasterServerOverride(DnsEndPoint::New_ctor<il2cpp_utils::CreationType::Manual>(il2cpp_utils::newcsstr(hostName), port));
		SetMasterServerOverride(DnsEndPoint::New_ctor(il2cpp_utils::newcsstr(hostName), port));
	}

	void MpConnect::SetMasterServerOverride(DnsEndPoint* overrideEndPoint) {
		if (!OverrideEndPoint || !(csstrtostr(OverrideEndPoint->ToString()) == csstrtostr(overrideEndPoint->ToString())))
		{
			//il2cpp_functions::GC_free(static_cast<DnsEndPoint*>(OverrideEndPoint));
			getLogger().info("Setting master server override now: %s", to_utf8(csstrtostr(overrideEndPoint->ToString())).c_str());
			OverrideEndPoint = overrideEndPoint;
		}
		else {
			//il2cpp_functions::GC_free(overrideEndPoint);
		}
	}

	void MpConnect::ClearMasterServerOverride() {
		if (OverrideEndPoint)
		{
			getLogger().info("Stopped overriding master server");
			//il2cpp_functions::GC_free(static_cast<DnsEndPoint*>(OverrideEndPoint));
			OverrideEndPoint = nullptr;
		}
	}
#pragma endregion
}