#ifndef REALISTIC_RENDERING_SCENE_CPP
#define REALISTIC_RENDERING_SCENE_CPP

#include <component/MeshRenderer.h>
#include <component/Camera.h>
#include <component/SpotLight.h>
#include <component/PointLight.h>
#include <component/PointRenderer.h>
#include <component/AmbientLight.h>
#include <component/LightProbe.h>
#include <resource/Texture.h>
#include <resource/Scene.h>
#include <resource/shader/BasicShader.h>
#include <resource/shader/UnlitShader.h>
#include <resource/shader/IrradianceShader.h>
#include <resource/shader/TransparentShader.h>
#include <resource/shader/SkyBoostShader.h>
#include <resource/shader/GShader.h>

#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/DOFProcessor.cpp"
#include "../PostProcessors/TAAProcessor.cpp"
#include "../Scripts/MaterialScript.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/BlurProcessor.cpp"
#include "../Scripts/PrinterScript.cpp"
#include "../Scripts/TransformerScript.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/StateControllerScript.cpp"
#include "../Scripts/PathWalkerScript.cpp"
#include "../Scripts/ControllerToggleScript.cpp"
#include "../Shader/BlurShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class RealisticRenderingScene : public Scene
{
public:

	void Load()
	{
		/* --------------------------- ENVIROMENT --------------------------- */
		
		std::vector<std::string> white = {
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
			Resources::ENGINE_RESOURCES + "/default_ao.png",
		};
		

		Renderer::setSkyBox(createResource<CubeMap>(white));
		Renderer::getSkyBox()->intensity = 10;


		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao					= new SSAOProcessor(10);
		SSReflectionProcessor* p_ssreflection	= new SSReflectionProcessor(20);
		AutoExposureProcessor* p_autoExposure	= new AutoExposureProcessor(30);
		TAAProcessor* p_fxaa					= new TAAProcessor(40);
		DOFProcessor* p_dof						= new DOFProcessor(50);
		BloomProcessor* p_bloom					= new BloomProcessor(60);
		ToneMappingProcessor* p_tonemapping		= new ToneMappingProcessor(70);
		p_dof->setEnabled(false);


		/* --------------------------- OBJ --------------------------- */
		Mesh* sphere			= createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		Mesh* couchMesh			= createResource<Mesh>("./res/Realistic-Rendering/Couch/couch.obj");
		Mesh* wallMesh			= createResource<Mesh>("./res/Realistic-Rendering/Walls/Room.obj");
		Mesh* floorMesh			= createResource<Mesh>("./res/Realistic-Rendering/Floor/Floor.obj");
		Mesh* doorsMesh			= createResource<Mesh>("./res/Realistic-Rendering/Doors/Doors.obj");
		Mesh* tableMesh			= createResource<Mesh>("./res/Realistic-Rendering/Table/Table.obj");
		Mesh* longCouchMesh		= createResource<Mesh>("./res/Realistic-Rendering/Couch/Long_Couch.obj");
		Mesh* slidingDoorMesh	= createResource<Mesh>("./res/Realistic-Rendering/SlidingDoor/SlideDoor.obj");
		Mesh* floorEdgeMesh		= createResource<Mesh>("./res/Realistic-Rendering/FloorEdge/FloorEdge.obj");
		Mesh* shelveMesh		= createResource<Mesh>("./res/Realistic-Rendering/Shelving/Shelving.obj");
		Mesh* carpetMesh		= createResource<Mesh>("./res/Realistic-Rendering/Carpet/Carpet.obj");
		Mesh* carpetFloorMesh	= createResource<Mesh>("./res/Realistic-Rendering/Carpet/CarpetFloor.obj");
		Mesh* occluderMesh		= createResource<Mesh>("./res/Realistic-Rendering/Occluder/occluder.obj");
		Mesh* windowMesh		= createResource<Mesh>("./res/Realistic-Rendering/Window/Window.obj");
		Mesh* panoramaMesh		= createResource<Mesh>("./res/Realistic-Rendering/Panorama/Panorama.obj");
		Mesh* lampMesh			= createResource<Mesh>("./res/Realistic-Rendering/Lamp/Lamp.obj");
		Mesh* ceilingLightMesh	= createResource<Mesh>("./res/Realistic-Rendering/CeilingLight/CeilingLight.obj");
		Mesh* vaseMesh			= createResource<Mesh>("./res/Realistic-Rendering/Vase/Vase.obj");
		Mesh* bookMesh1			= createResource<Mesh>("./res/Realistic-Rendering/Books/Books_1.obj");
		Mesh* bookMesh2			= createResource<Mesh>("./res/Realistic-Rendering/Books/Books_2.obj");
		Mesh* bookMesh3			= createResource<Mesh>("./res/Realistic-Rendering/Books/Books_3.obj");
		Mesh* bookMesh4			= createResource<Mesh>("./res/Realistic-Rendering/Books/Books_4.obj");
		Mesh* balconyFloorMesh	= createResource<Mesh>("./res/Realistic-Rendering/BalconyFloor/Balcony_Floor.obj");
		Mesh* balconyRailingMesh = createResource<Mesh>("./res/Realistic-Rendering/Railing/Balcony_Railing.obj");
		Mesh* frameMesh			= createResource<Mesh>("./res/Realistic-Rendering/Frame/Frame.obj");
		Mesh* frameGlassMesh	= createResource<Mesh>("./res/Realistic-Rendering/Frame/GlassPane.obj");
		Mesh* painting1Mesh		= createResource<Mesh>("./res/Realistic-Rendering/Frame/Painting1.obj");
		Mesh* painting2Mesh		= createResource<Mesh>("./res/Realistic-Rendering/Frame/Painting2.obj");
		Mesh* painting3Mesh		= createResource<Mesh>("./res/Realistic-Rendering/Frame/Painting3.obj");


		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);

		Texture* woodAlbedo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_basecolor.png"), true);
		Texture* woodRough = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_roughness.png"), true);
		Texture* woodNormal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_normal.png"), true);
		Texture* woodAo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_AO.png"), true);
		Texture* woodMetal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_metalness.png"), true);
		
		Texture* doorAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Doors/T_Door_D.tga"), true);
		Texture* doorRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Doors/T_Door_R.tga"), true);
		Texture* doorNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Doors/T_Door_N.TGA"), true);
		Texture* doorAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Doors/T_Door_AOMask.TGA"), true);
		Texture* doorMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/Doors/T_Door_M.tga"), true);
		
		Texture* tableAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Table/T_Table_D.tga"), true);
		Texture* tableRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Table/T_Table_R.tga"), true);
		Texture* tableNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Table/T_Table_N.TGA"), true);
		Texture* tableAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Table/T_Table_AOMASK.TGA"), true);
		Texture* tableMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/Table/T_Table_M.tga"), true);
		
		Texture* slidingDoorRough = createResource<Texture>(std::string("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_R.tga"), true);
		Texture* slidingDoorNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_N.TGA"), true);
		Texture* slidingDoorAo = createResource<Texture>(std::string("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_AOMask.TGA"), true);
		Texture* slidingDoorMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/SlidingDoor/T_SlidingDoor_M.TGA"), true);
		
		Texture* shelveAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Shelving/T_Shelving_D.TGA"), true);
		Texture* shelveRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Shelving/T_Shelving_R.TGA"), true);
		Texture* shelveNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Shelving/T_Shelving_N.TGA"), true);
		Texture* shelveAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Shelving/T_Shelving_AO.TGA"), true);
		
		Texture* carpetAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Carpet/T_Carpet_D.png"), true);
		Texture* carpetNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Carpet/T_Carpet_N.TGA"), true);
		Texture* carpetAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Carpet/T_Carpet_AO.TGA"), true);
		 
		Texture* lampAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Lamp/T_FloorLamp_Albedo.TGA"), true);
		Texture* lampRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Lamp/T_FloorLamp_Roughness.TGA"), true);
		Texture* lampNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Lamp/T_FloorLamp_N.TGA"), true);
		Texture* lampAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Lamp/T_FloorLamp_AO.TGA"), true);
		Texture* lampMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/Lamp/T_FloorLamp_M.TGA"), true);
		
		Texture* ceilingAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Albedo.TGA"), true);
		Texture* ceilingRough = createResource<Texture>(std::string("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_Roughness.TGA"), true);
		Texture* ceilingNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_N.TGA"), true);
		Texture* ceilingAo = createResource<Texture>(std::string("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_AO.TGA"), true);
		Texture* ceilingMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/CeilingLight/T_RoundCeilingLight_M.TGA"), true);
		
		Texture* vaseAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Vase/Vase_Albedo.png"), true);
		Texture* vaseRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Vase/Vase_Roughness.png"), true);
		
		Texture* frameAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Frame_Albedo.tga"), true);
		Texture* frameNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Frame_N.tga"), true);
		Texture* frameRoughness = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Frame_Roughness.tga"), true);
		
		Texture* painting1D = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Painting4_D.tga"), true);
		Texture* painting2D = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Painting2_D.tga"), true);
		Texture* painting3D = createResource<Texture>(std::string("./res/Realistic-Rendering/Frame/T_Painting3_D.tga"), true);
		
		Texture* bookAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_occlusion.tga"), true);
		Texture* bookNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_normals.tga"), true);
		Texture* bookAlbedo1 = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_albedo_1.tga"), true);
		Texture* bookAlbedo2 = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_albedo_2.tga"), true);
		Texture* bookAlbedo3 = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_albedo_3.tga"), true);
		Texture* bookAlbedo4 = createResource<Texture>(std::string("./res/Realistic-Rendering/Books/book_albedo_4.tga"), true);
		
		Texture* balconyFloorAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_D.tga"), true);
		Texture* balconyFloorNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_N.tga"), true);
		Texture* balconyFloorRoughness = createResource<Texture>(std::string("./res/Realistic-Rendering/BalconyFloor/T_StoneMix_R.tga"), true);
		
		Texture* galvanizedAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Railing/T_Galvanized_D.tga"), true);
		Texture* galvanizedRough = createResource<Texture>(std::string("./res/Realistic-Rendering/Railing/T_Galvanized_Roughness.tga"), true);
		Texture* galvanizedNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Railing/T_Galvanized_N.tga"), true);
		Texture* railingAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Railing/T_Railing_AOMASK.tga"), true);
		Texture* railingNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Railing/T_Railing_N.tga"), true);
		
		Texture* windowAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Window/albedo.png"), true);
		
		Texture* couchAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Couch/T_Couch_D.TGA"), true);
		Texture* couchNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Couch/T_Couch_N.TGA"), true);
		Texture* couchRoughness = createResource<Texture>(std::string("./res/Realistic-Rendering/Couch/T_Couch_R.tga"), true);
		Texture* couchAo = createResource<Texture>(std::string("./res/Realistic-Rendering/Couch/T_Couch_AO.TGA"), true);
		Texture* couchMetal = createResource<Texture>(std::string("./res/Realistic-Rendering/Couch/T_Couch_M.TGA"), true);
		
		Texture* wallNormal = createResource<Texture>(std::string("./res/Realistic-Rendering/Walls/T_PaintedWall_N.TGA"), true);
		
		Texture* panoramaAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Panorama/T_Background_D.TGA"), true);
		Texture* blinds = createResource<Texture>(std::string("./res/textures/blinds.png"), true);


		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader = GShader::getInstance();
		TransparentShader* transparentShader = TransparentShader::getInstance();
		UnlitShader* unlitShader = UnlitShader::getInstance();
		SkyBoostShader* skyBoost = SkyBoostShader::getInstance();


		/* --------------------------- MATERIALS --------------------------- */

		Material* couchMaterial = createResource<Material>(defaultShader);
		couchMaterial->setTexture("albedoMap", couchAlbedo);
		couchMaterial->setTexture("normalMap", couchNormal);
		couchMaterial->setTexture("roughnessMap", couchRoughness);
		couchMaterial->setTexture("aoMap", couchAo);
		couchMaterial->setTexture("metallicMap", couchMetal);
		couchMaterial->setTexture("emissionMap", default_emission);

		Material* wallMaterial = createResource<Material>(defaultShader);
		wallMaterial->setTexture("albedoMap", default_ao);
		wallMaterial->setTexture("normalMap", wallNormal);
		wallMaterial->setTexture("roughnessMap", default_ao);
		wallMaterial->setTexture("aoMap", default_ao);
		wallMaterial->setTexture("metallicMap", default_emission);
		wallMaterial->setTexture("emissionMap", default_emission);

		Material* wood = createResource<Material>(defaultShader);
		wood->setTexture("albedoMap", woodAlbedo);
		wood->setTexture("normalMap", woodNormal);
		wood->setTexture("roughnessMap", woodRough);
		wood->setTexture("aoMap", woodAo);
		wood->setTexture("metallicMap", woodMetal);
		wood->setTexture("emissionMap", default_emission);

		Material* doorMaterial = createResource<Material>(defaultShader);
		doorMaterial->setTexture("albedoMap", doorAlbedo);
		doorMaterial->setTexture("normalMap", doorNormal);
		doorMaterial->setTexture("roughnessMap", doorRough);
		doorMaterial->setTexture("aoMap", doorAo);
		doorMaterial->setTexture("metallicMap", doorMetal);
		doorMaterial->setTexture("emissionMap", default_emission);

		Material* tableMaterial = createResource<Material>(defaultShader);
		tableMaterial->setTexture("albedoMap", tableAlbedo);
		tableMaterial->setTexture("normalMap", tableNormal);
		tableMaterial->setTexture("roughnessMap", tableRough);
		tableMaterial->setTexture("aoMap", tableAo);
		tableMaterial->setTexture("metallicMap", tableMetal);
		tableMaterial->setTexture("emissionMap", default_emission);

		Material* shelveMaterial = createResource<Material>(defaultShader);
		shelveMaterial->setTexture("albedoMap", shelveAlbedo);
		shelveMaterial->setTexture("normalMap", shelveNormal);
		shelveMaterial->setTexture("roughnessMap", shelveRough);
		shelveMaterial->setTexture("aoMap", shelveAo);
		shelveMaterial->setTexture("metallicMap", default_emission);
		shelveMaterial->setTexture("emissionMap", default_emission);

		Material* slidingDoorMaterial = createResource<Material>(defaultShader);
		slidingDoorMaterial->setTexture("albedoMap", default_ao);
		slidingDoorMaterial->setTexture("normalMap", slidingDoorNormal);
		slidingDoorMaterial->setTexture("roughnessMap", slidingDoorRough);
		slidingDoorMaterial->setTexture("aoMap", slidingDoorAo);
		slidingDoorMaterial->setTexture("metallicMap", slidingDoorMetal);
		slidingDoorMaterial->setTexture("emissionMap", default_emission);

		Material* carpetMaterial = createResource<Material>(defaultShader);
		carpetMaterial->setTexture("albedoMap", carpetAlbedo);
		carpetMaterial->setTexture("normalMap", default_normal);
		carpetMaterial->setTexture("roughnessMap", default_ao);
		carpetMaterial->setTexture("aoMap", carpetAo);
		carpetMaterial->setTexture("metallicMap", default_emission);
		carpetMaterial->setTexture("emissionMap", default_emission);

		Material* carpetFloorMaterial = createResource<Material>(defaultShader);
		carpetFloorMaterial->setTexture("albedoMap", carpetAlbedo);
		carpetFloorMaterial->setTexture("normalMap", default_normal);
		carpetFloorMaterial->setTexture("roughnessMap", default_ao);
		carpetFloorMaterial->setTexture("aoMap", carpetAo);
		carpetFloorMaterial->setTexture("metallicMap", default_emission);
		carpetFloorMaterial->setTexture("emissionMap", default_emission);

		Material* lampMaterial = createResource<Material>(defaultShader);
		lampMaterial->setTexture("albedoMap", lampAlbedo);
		lampMaterial->setTexture("normalMap", lampNormal);
		lampMaterial->setTexture("roughnessMap", lampRough);
		lampMaterial->setTexture("aoMap", lampAo);
		lampMaterial->setTexture("metallicMap", lampMetal);
		lampMaterial->setTexture("emissionMap", default_emission);

		Material* ceilingLightMaterial = createResource<Material>(defaultShader);
		ceilingLightMaterial->setTexture("albedoMap", ceilingAlbedo);
		ceilingLightMaterial->setTexture("normalMap", ceilingNormal);
		ceilingLightMaterial->setTexture("roughnessMap", ceilingRough);
		ceilingLightMaterial->setTexture("aoMap", ceilingAo);
		ceilingLightMaterial->setTexture("metallicMap", ceilingMetal);
		ceilingLightMaterial->setTexture("emissionMap", default_emission);

		Material* vaseMaterial = createResource<Material>(defaultShader);
		vaseMaterial->setTexture("albedoMap", vaseAlbedo);
		vaseMaterial->setTexture("normalMap", default_normal);
		vaseMaterial->setTexture("roughnessMap", vaseRough);
		vaseMaterial->setTexture("aoMap", default_ao);
		vaseMaterial->setTexture("metallicMap", default_emission);
		vaseMaterial->setTexture("emissionMap", default_emission);

		Material* frameMaterial = createResource<Material>(defaultShader);
		frameMaterial->setTexture("albedoMap", frameAlbedo);
		frameMaterial->setTexture("normalMap", frameNormal);
		frameMaterial->setTexture("roughnessMap", frameRoughness);
		frameMaterial->setTexture("aoMap", default_ao);
		frameMaterial->setTexture("metallicMap", default_emission);
		frameMaterial->setTexture("emissionMap", default_emission);

		Material* painting1Material = createResource<Material>(defaultShader);
		painting1Material->setTexture("albedoMap", painting1D);
		painting1Material->setTexture("normalMap", default_normal);
		painting1Material->setTexture("roughnessMap", default_ao);
		painting1Material->setTexture("aoMap", default_ao);
		painting1Material->setTexture("metallicMap", default_emission);
		painting1Material->setTexture("emissionMap", default_emission);
		Material* painting2Material = createResource<Material>(defaultShader);
		painting2Material->setTexture("albedoMap", painting2D);
		painting2Material->setTexture("normalMap", default_normal);
		painting2Material->setTexture("roughnessMap", default_ao);
		painting2Material->setTexture("aoMap", default_ao);
		painting2Material->setTexture("metallicMap", default_emission);
		painting2Material->setTexture("emissionMap", default_emission);
		Material* painting3Material = createResource<Material>(defaultShader);
		painting3Material->setTexture("albedoMap", painting3D);
		painting3Material->setTexture("normalMap", default_normal);
		painting3Material->setTexture("roughnessMap", default_ao);
		painting3Material->setTexture("aoMap", default_ao);
		painting3Material->setTexture("metallicMap", default_emission);
		painting3Material->setTexture("emissionMap", default_emission);


		Material* bookMaterial1 = createResource<Material>(defaultShader);
		bookMaterial1->setTexture("albedoMap", bookAlbedo1);
		bookMaterial1->setTexture("normalMap", bookNormal);
		bookMaterial1->setTexture("roughnessMap", default_ao);
		bookMaterial1->setTexture("aoMap", bookAo);
		bookMaterial1->setTexture("metallicMap", default_emission);
		bookMaterial1->setTexture("emissionMap", default_emission);
		Material* bookMaterial2 = createResource<Material>(defaultShader);
		bookMaterial2->setTexture("albedoMap", bookAlbedo2);
		bookMaterial2->setTexture("normalMap", bookNormal);
		bookMaterial2->setTexture("roughnessMap", default_ao);
		bookMaterial2->setTexture("aoMap", bookAo);
		bookMaterial2->setTexture("metallicMap", default_emission);
		bookMaterial2->setTexture("emissionMap", default_emission);
		Material* bookMaterial3 = createResource<Material>(defaultShader);
		bookMaterial3->setTexture("albedoMap", bookAlbedo3);
		bookMaterial3->setTexture("normalMap", bookNormal);
		bookMaterial3->setTexture("roughnessMap", default_ao);
		bookMaterial3->setTexture("aoMap", bookAo);
		bookMaterial3->setTexture("metallicMap", default_emission);
		bookMaterial3->setTexture("emissionMap", default_emission);
		Material* bookMaterial4 = createResource<Material>(defaultShader);
		bookMaterial4->setTexture("albedoMap", bookAlbedo4);
		bookMaterial4->setTexture("normalMap", bookNormal);
		bookMaterial4->setTexture("roughnessMap", default_ao);
		bookMaterial4->setTexture("aoMap", bookAo);
		bookMaterial4->setTexture("metallicMap", default_emission);
		bookMaterial4->setTexture("emissionMap", default_emission);

		Material* railingMaterial = createResource<Material>(defaultShader);
		railingMaterial->setTexture("albedoMap", galvanizedAlbedo);
		railingMaterial->setTexture("normalMap", railingNormal);
		railingMaterial->setTexture("roughnessMap", galvanizedRough);
		railingMaterial->setTexture("aoMap", railingAo);
		railingMaterial->setTexture("metallicMap", default_ao);
		railingMaterial->setTexture("emissionMap", default_emission);

		Material* balconyFloorMaterial = createResource<Material>(defaultShader);
		balconyFloorMaterial->setTexture("albedoMap", balconyFloorAlbedo);
		balconyFloorMaterial->setTexture("normalMap", balconyFloorNormal);
		balconyFloorMaterial->setTexture("roughnessMap", balconyFloorRoughness);
		balconyFloorMaterial->setTexture("aoMap", default_ao);
		balconyFloorMaterial->setTexture("metallicMap", default_emission);
		balconyFloorMaterial->setTexture("emissionMap", default_emission);

		Material* blindsMaterial = createResource<Material>(defaultShader);
		blindsMaterial->setTexture("albedoMap", blinds);
		blindsMaterial->setTexture("normalMap", default_normal);
		blindsMaterial->setTexture("roughnessMap", default_emission);
		blindsMaterial->setTexture("aoMap", default_ao);
		blindsMaterial->setTexture("metallicMap", default_ao);
		blindsMaterial->setTexture("emissionMap", default_emission);

		Material* def = createResource<Material>(defaultShader);
		def->setTexture("albedoMap", default_specular);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_emission);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_ao);
		def->setTexture("emissionMap", default_emission);

		Material* glassMaterial = createResource<Material>(transparentShader);
		glassMaterial->setTexture("albedoMap", windowAlbedo);
		glassMaterial->setTexture("normalMap", default_normal);
		glassMaterial->setTexture("roughnessMap", default_roughness);
		glassMaterial->setTexture("aoMap", default_ao);
		glassMaterial->setTexture("metallicMap", default_emission);
		glassMaterial->setTexture("emissionMap", default_emission);

		Material* occluderMaterial = createResource<Material>(unlitShader);

		Material* panoramaMaterial = createResource<Material>(skyBoost);
		panoramaMaterial->setTexture("albedoMap", panoramaAlbedo);
		panoramaMaterial->setProperty("boost", 60.0f);

		/* --------------------------- CAMERA --------------------------- */

		std::vector<Vector3f> positions;
		std::vector<Quaternion> rotations;
		
		positions.push_back(Vector3f(1.027929f, 0.694633f, -2.158259f));
		rotations.push_back(Quaternion(0.000000f, 0.000000f, 0.000000f, 1.000000f));

		positions.push_back(Vector3f(-1.360401f, 0.694633f, -2.075065f));
		rotations.push_back(Quaternion(-0.028378f, -0.230313f, 0.006719f, 0.972680f));

		positions.push_back(Vector3f(-2.056820f, 0.512246f, -0.024637f));
		rotations.push_back(Quaternion(0.009905f, -0.705615f, -0.006306f, 0.703909f));

		positions.push_back(Vector3f(-1.386565f, 0.527661f, -0.021926f));
		rotations.push_back(Quaternion(0.009905f, -0.705615f, -0.006306f, 0.703909f));

		positions.push_back(Vector3f(-1.546286f, 1.844576f, 3.710133f));
		rotations.push_back(Quaternion(-0.003881f, 0.921661f, -0.005412f, 0.374513f));

		positions.push_back(Vector3f(-1.551588f, 0.697409f, 3.701233f));
		rotations.push_back(Quaternion(0.083715f, 0.875332f, 0.169155f, 0.433270f));

		positions.push_back(Vector3f(1.669045f, 1.679932f, -0.204936f));
		rotations.push_back(Quaternion(-0.001640f, 0.990844f, -0.011507f, -0.018861f));

		positions.push_back(Vector3f(1.831609f, 1.694534f, 0.390910f));
		rotations.push_back(Quaternion(-0.001640f, 0.990844f, -0.011507f, -0.018861f));

		positions.push_back(Vector3f(1.302423f, 1.878779f, 3.948425f));
		rotations.push_back(Quaternion(0.024274f, -0.972819f, 0.122343f, -0.131675f));

		positions.push_back(Vector3f(0.219692f, 1.831888f, 3.995026f));
		rotations.push_back(Quaternion(-0.001262f, -0.979887f, 0.116091f, 0.074467f));

		positions.push_back(Vector3f(1.450421f, 2.951797f, -1.219468f));
		rotations.push_back(Quaternion(0.450830f, -0.545559f, 0.454648f, -0.519226f));

		positions.push_back(Vector3f(1.427990f, 2.900677f, 1.304277f));
		rotations.push_back(Quaternion(0.450830f, -0.545559f, 0.454648f, -0.519226f));

		positions.push_back(Vector3f(0.001378f, 0.554877f, 1.378379f));
		rotations.push_back(Quaternion(0.000040f, -0.999493f, -0.020330f, -0.000862f));

		positions.push_back(Vector3f(-0.001278f, 0.617471f, -0.158748f));
		rotations.push_back(Quaternion(0.000040f, -0.999493f, -0.020330f, -0.000862f));



		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>()->setFOV(60);
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 2, -2));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();
		
		cam->addComponent<PathWalkerScript>()->speed = 0.1f;
		cam->getComponent<PathWalkerScript>()->positions = positions;
		cam->getComponent<PathWalkerScript>()->rotations = rotations;	
		cam->addComponent<ControllerToggleScript>();
		


		/* --------------------------- LIGHT HELPER --------------------------- */
		
		Entity* lightProbe = instanciate("LightProbe");
		lightProbe->getComponent<Transform>()->setPosition(Vector3f(0, 1.2f, 0));
		lightProbe->addComponent<LightProbe>()->resolution = 512;
		lightProbe->getComponent<LightProbe>()->bounds.setMinBound(Vector3f(-2.1f, -1.3f, -3.4f));
		lightProbe->getComponent<LightProbe>()->bounds.setMaxBound(Vector3f(2.1f, 2.0f, 6));
		

		Entity* min = instanciate("min");
		min->getComponent<Transform>()->setPosition(Vector3f(-2.1f, -1.3f, -3.4f));

		Entity* max = instanciate("max");
		max->getComponent<Transform>()->setPosition(Vector3f(2.1f, 2.0f, 6));


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;

		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(255.0f/255.0f, 248.0f / 255.0f, 240.0f / 255.0f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(0.4f, -0.6f, 2.0f);
		sun->getComponent<DirectionalLight>()->intensity = 800.0f;



		/* --------------------------- OBJECTSS --------------------------- */

		Entity* couch = instanciate("Couch");
		couch->addComponent<MeshRenderer>();
		couch->getComponent<MeshRenderer>()->setMesh(couchMesh);
		couch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		Entity* longCouch = instanciate("Long-Couch");
		longCouch->addComponent<MeshRenderer>();
		longCouch->getComponent<MeshRenderer>()->setMesh(longCouchMesh);
		longCouch->getComponent<MeshRenderer>()->setMaterial(couchMaterial);

		Entity* table = instanciate("Rable");
		table->addComponent<MeshRenderer>()->getReflected = false;
		table->getComponent<MeshRenderer>()->setMesh(tableMesh);
		table->getComponent<MeshRenderer>()->setMaterial(tableMaterial);

		Entity* shelve = instanciate("Shelve");
		shelve->addComponent<MeshRenderer>();
		shelve->getComponent<MeshRenderer>()->setMesh(shelveMesh);
		shelve->getComponent<MeshRenderer>()->setMaterial(shelveMaterial);

		Entity* room = instanciate("Room");
		room->addComponent<MeshRenderer>();
		room->getComponent<MeshRenderer>()->setMesh(wallMesh);
		room->getComponent<MeshRenderer>()->setMaterial(wallMaterial);

		Entity* occluder = instanciate("Occluder");
		occluder->addComponent<MeshRenderer>();
		occluder->getComponent<MeshRenderer>()->setMesh(occluderMesh);
		occluder->getComponent<MeshRenderer>()->setMaterial(occluderMaterial);

		Entity* floor = instanciate("Floor");
		floor->addComponent<MeshRenderer>();
		floor->getComponent<MeshRenderer>()->setMesh(floorMesh);
		floor->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* floorEdge = instanciate("FloorEdge");
		floorEdge->addComponent<MeshRenderer>();
		floorEdge->getComponent<MeshRenderer>()->setMesh(floorEdgeMesh);
		floorEdge->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* doors = instanciate("Doors");
		doors->addComponent<MeshRenderer>();
		doors->getComponent<MeshRenderer>()->setMesh(doorsMesh);
		doors->getComponent<MeshRenderer>()->setMaterial(doorMaterial);

		Entity* slidingDoor = instanciate("Sliding-Doors");
		slidingDoor->addComponent<MeshRenderer>();
		slidingDoor->getComponent<MeshRenderer>()->setMesh(slidingDoorMesh);
		slidingDoor->getComponent<MeshRenderer>()->setMaterial(slidingDoorMaterial);

		Entity* carpet = instanciate("Carpet");
		carpet->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_CUTOUT);
		carpet->getComponent<MeshRenderer>()->setMesh(carpetMesh);
		carpet->getComponent<MeshRenderer>()->setMaterial(carpetMaterial);

		Entity* carpetFloor = instanciate("CarpetFloor");
		carpetFloor->addComponent<MeshRenderer>();
		carpetFloor->getComponent<MeshRenderer>()->setMesh(carpetFloorMesh);
		carpetFloor->getComponent<MeshRenderer>()->setMaterial(carpetFloorMaterial);

		Entity* lamp = instanciate("Lamp");
		lamp->addComponent<MeshRenderer>();
		lamp->getComponent<MeshRenderer>()->setMesh(lampMesh);
		lamp->getComponent<MeshRenderer>()->setMaterial(lampMaterial);

		Entity* ceilingLight = instanciate("CeilingLight");
		ceilingLight->addComponent<MeshRenderer>();
		ceilingLight->getComponent<MeshRenderer>()->setMesh(ceilingLightMesh);
		ceilingLight->getComponent<MeshRenderer>()->setMaterial(ceilingLightMaterial);

		Entity* vase = instanciate("Vase");
		vase->addComponent<MeshRenderer>();
		vase->getComponent<MeshRenderer>()->setMesh(vaseMesh);
		vase->getComponent<MeshRenderer>()->setMaterial(vaseMaterial);

		Entity* frame = instanciate("Frame");
		frame->addComponent<MeshRenderer>();
		frame->getComponent<MeshRenderer>()->setMesh(frameMesh);
		frame->getComponent<MeshRenderer>()->setMaterial(frameMaterial);

		Entity* frameGlass = instanciate("FrameGlass");
		frameGlass->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		frameGlass->getComponent<MeshRenderer>()->setMesh(frameGlassMesh);
		frameGlass->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* painting1 = instanciate("Painting1");
		painting1->addComponent<MeshRenderer>();
		painting1->getComponent<MeshRenderer>()->setMesh(painting1Mesh);
		painting1->getComponent<MeshRenderer>()->setMaterial(painting1Material);

		Entity* painting2 = instanciate("Painting2");
		painting2->addComponent<MeshRenderer>();
		painting2->getComponent<MeshRenderer>()->setMesh(painting2Mesh);
		painting2->getComponent<MeshRenderer>()->setMaterial(painting2Material);

		Entity* painting3 = instanciate("Painting3");
		painting3->addComponent<MeshRenderer>();
		painting3->getComponent<MeshRenderer>()->setMesh(painting3Mesh);
		painting3->getComponent<MeshRenderer>()->setMaterial(painting3Material);


		Entity* book1 = instanciate("Book1");
		book1->addComponent<MeshRenderer>();
		book1->getComponent<MeshRenderer>()->setMesh(bookMesh1);
		book1->getComponent<MeshRenderer>()->setMaterial(bookMaterial1);
		Entity* book2 = instanciate("Book2");
		book2->addComponent<MeshRenderer>();
		book2->getComponent<MeshRenderer>()->setMesh(bookMesh2);
		book2->getComponent<MeshRenderer>()->setMaterial(bookMaterial2);
		Entity* book3 = instanciate("Book3");
		book3->addComponent<MeshRenderer>();
		book3->getComponent<MeshRenderer>()->setMesh(bookMesh3);
		book3->getComponent<MeshRenderer>()->setMaterial(bookMaterial3);
		Entity* book4 = instanciate("Book4");
		book4->addComponent<MeshRenderer>();
		book4->getComponent<MeshRenderer>()->setMesh(bookMesh4);
		book4->getComponent<MeshRenderer>()->setMaterial(bookMaterial4);

		Entity* balconyFloor = instanciate("Balcony Floor");
		balconyFloor->addComponent<MeshRenderer>();
		balconyFloor->getComponent<MeshRenderer>()->setMesh(balconyFloorMesh);
		balconyFloor->getComponent<MeshRenderer>()->setMaterial(balconyFloorMaterial);

		Entity* balconyRailing = instanciate("Balcony Railing");
		balconyRailing->addComponent<MeshRenderer>();
		balconyRailing->getComponent<MeshRenderer>()->setMesh(balconyRailingMesh);
		balconyRailing->getComponent<MeshRenderer>()->setMaterial(railingMaterial);

		Entity* probe = instanciate("Probe");
		probe->getComponent<Transform>()->setPosition(Vector3f(0.0f, 1.2f, 0.0f));
		probe->getComponent<Transform>()->setScale(Vector3f(0.1f, 0.1f, 0.1f));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(def);
		probe->getComponent<MeshRenderer>()->getReflected = false;
		
		Entity* window = instanciate("Window");
		window->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		window->getComponent<MeshRenderer>()->setMesh(windowMesh);
		window->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* panorama = instanciate("Panorama");
		panorama->addComponent<MeshRenderer>();
		panorama->getComponent<MeshRenderer>()->setMesh(panoramaMesh);
		panorama->getComponent<MeshRenderer>()->setMaterial(panoramaMaterial);
	}

};
#endif // REALISTIC_RENDERING_SCENE_CPP