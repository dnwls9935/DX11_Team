#include "pch.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "Image.h"
#include "Transform.h"
#include "RectTransform.h"
#include "AudioListener.h"
#include "CETween.h"
#include "TopBar.h"
#include "OptionPopup.h"
#include "FadeController.h"
#include "BGMPlayer.h"
#include "Application.h"

void TitleScene::SetLoadingContents(void) noexcept
{

}

HRESULT TitleScene::Init(void) noexcept
{
	// Load Asset
	ASSETMANAGER->LoadDirectory("Asset\\UI\\");
	ASSETMANAGER->LoadDirectory("Asset\\Audio\\UI\\");
	ASSETMANAGER->LoadDirectory("Asset\\Font\\");

	// Set Resolution
	int resolutionIndex = PlayerPrefs::This().GetInt("Resolution", 5);
	int width = 0;
	int height = 0;
	switch (resolutionIndex)
	{
	case 0:
		width = 1120;
		height = 630;
		break;
	case 1:
		width = 1280;
		height = 720;
		break;
	case 2:
		width = 1440;
		height = 810;
		break;
	case 3:
		width = 1600;
		height = 900;
		break;
	case 4:
		width = 1760;
		height = 990;
		break;
	case 5:
		width = 1920;
		height = 1080;
		break;
	case 6:
		width = 2560;
		height = 1440;
		break;
	}

	int xPos = (Application::This().GetScreenWidth() - width) / 2;
	int yPos = (Application::This().GetScreenHeight() - height) / 2;

	SetWindowPos(Application::This().GetHandle()
		, NULL
		, xPos
		, yPos
		, width
		, height
		, SWP_NOZORDER);

	// Create TopBar
	GameObject* obj = GameObject::Instantiate("TopBar");
	obj->AddComponent(new TopBar());
	obj->SetDontDestroy(true);

	// Play BGM
	int bgmindex = BGMPlayer::This()->AddBGM("Asset\\Audio\\MainMenuTheme.mp3", "MainMenu");
	float volume = PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("MusicVolume", 5);
	BGMPlayer::This()->SetVolume(volume, bgmindex);
	BGMPlayer::This()->Play(bgmindex);

	// Title Background
	obj = GameObject::Instantiate();
	UI::Image* image = UI::Image::Create();
	image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\Title.dds"));
	obj->AddComponent(image);
	obj->GetComponent<RectTransform>()->SetHeight(WINCY);
	obj->GetComponent<RectTransform>()->SetWidth(WINCX);
	obj->AddComponent(AudioListener::Create());

	// Button Effect Sound
	float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);

	_btnAudio = AudioSource::Create();
	_btnAudio->LoadAudio(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\UI\\Button\\Play_UI_OptionsMenu_TopBar_ChangeSelection.ogg"));
	_btnAudio->SetVolume(effectVolume);
	obj->AddComponent(_btnAudio);

	_btnOverAudio = AudioSource::Create();
	_btnOverAudio->LoadAudio(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\UI\\Button\\Play_UI_Menu_ChangeSelection.ogg"));
	_btnOverAudio->SetVolume(effectVolume);
	obj->AddComponent(_btnOverAudio);

	// Logo Image
	obj = GameObject::Instantiate("Logo Img");
	image = UI::Image::Create();
	image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\Logo.dds"));
	obj->AddComponent(image);
	obj->GetComponent<RectTransform>()->SetHeight(400);
	obj->GetComponent<RectTransform>()->SetWidth(800);
	obj->GetComponent<RectTransform>()->SetPivot(0.5f, 0.5f);
	obj->GetTransform()->SetWorldPosition(WINCX * 0.25f, WINCY * 0.25f, 0);
	obj->SetSortOrder(1);
	_LogoTransform = obj->GetTransform();

	// Main Menu Buttons
	obj = GameObject::Instantiate("MainOption");
	image = UI::Image::Create();
	obj->AddComponent(image);
	obj->GetComponent<RectTransform>()->SetPivot(0, 0);
	obj->GetTransform()->SetWorldPosition(150, WINCY * 0.5f, 0);
	obj->SetSortOrder(1);

	{
		int i = 0;
		GameObject* childObj = GameObject::Instantiate("PlayLocal Btns");
		childObj->GetTransform()->SetParent(obj->GetTransform());
		image = UI::Image::Create();
		image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\MenuRowBox.dds"));
		childObj->AddComponent(image);
		childObj->GetComponent<RectTransform>()->SetPivot(0, 0.5f);
		childObj->GetComponent<RectTransform>()->SetHeight(64);
		childObj->GetComponent<RectTransform>()->SetWidth(WINCX * 0.25f);
		childObj->GetTransform()->SetLocalPosition(0, 50 + (i * 66), 0);
		childObj->SetSortOrder(2);
		image->SetColor(1, 1, 1, 0);

		GameObject* textObj = GameObject::Instantiate("PlayLocal Text");
		textObj->GetTransform()->SetParent(childObj->GetTransform());
		Text* text = Text::Create();
		text->SetString(L"이어하기");
		text->SetAligement(Text::Aligement::LEFT_MIDDLE);
		text->SetColor(1.f, 1.f, 1.0f, 1);
		text->SetSize(25);
		textObj->AddComponent(text);
		text->SetInteractable(false);
		textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		textObj->SetSortOrder(3);
		Button* btn = Button::Create();
		childObj->AddComponent(btn);

		btn->onMouseEnter += [&]() {
			float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
			_btnOverAudio->SetVolume(effectVolume);
			_btnOverAudio->Play();
			
			GameObject::FindObjectByName("PlayLocal Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 1);
			GameObject* textObj = GameObject::FindObjectByName("PlayLocal Text");
			textObj->GetComponent<UI::Text>()->SetColor(0.4f, 0.1f, 0.1f, 1);
			textObj->GetTransform()->SetLocalPosition(80, 0, 0);

		};

		btn->onMouseLeave += [&]() {
			GameObject::FindObjectByName("PlayLocal Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("PlayLocal Text");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		};

		btn->onMouseDown += std::bind(&TitleScene::OnClickPlayLocal, this);

		i++;

		childObj = GameObject::Instantiate("PlayOnline Btns");
		childObj->GetTransform()->SetParent(obj->GetTransform());
		image = UI::Image::Create();
		image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\MenuRowBox.dds"));
		childObj->AddComponent(image);
		childObj->GetComponent<RectTransform>()->SetPivot(0, 0.5f);
		childObj->GetComponent<RectTransform>()->SetHeight(64);
		childObj->GetComponent<RectTransform>()->SetWidth(WINCX * 0.25f);
		childObj->GetTransform()->SetLocalPosition(0, 50 + (i * 66), 0);
		childObj->SetSortOrder(2);
		image->SetColor(1, 1, 1, 0);

		textObj = GameObject::Instantiate("PlayOnline Text");
		textObj->GetTransform()->SetParent(childObj->GetTransform());
		text = Text::Create();
		text->SetString(L"새로하기");
		text->SetAligement(Text::Aligement::LEFT_MIDDLE);
		text->SetColor(1.f, 1.f, 1.0f, 1);
		text->SetSize(25);
		textObj->AddComponent(text);
		text->SetInteractable(false);
		textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		textObj->SetSortOrder(3);

		btn = Button::Create();
		childObj->AddComponent(btn);

		btn->onMouseEnter += [&]() {
			float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
			_btnOverAudio->SetVolume(effectVolume);
			_btnOverAudio->Play();
			
			GameObject::FindObjectByName("PlayOnline Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 1);
			GameObject* textObj = GameObject::FindObjectByName("PlayOnline Text");
			textObj->GetComponent<UI::Text>()->SetColor(0.4f, 0.1f, 0.1f, 1);
			textObj->GetTransform()->SetLocalPosition(80, 0, 0);
		};

		btn->onMouseLeave += [&]() {
			GameObject::FindObjectByName("PlayOnline Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("PlayOnline Text");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		};

		btn->onMouseDown += std::bind(&TitleScene::OnClickPlayOnline, this);

		i++;

		childObj = GameObject::Instantiate("Option Btns");
		childObj->GetTransform()->SetParent(obj->GetTransform());
		image = UI::Image::Create();
		image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\MenuRowBox.dds"));
		childObj->AddComponent(image);
		childObj->GetComponent<RectTransform>()->SetPivot(0, 0.5f);
		childObj->GetComponent<RectTransform>()->SetHeight(64);
		childObj->GetComponent<RectTransform>()->SetWidth(WINCX * 0.25f);
		childObj->GetTransform()->SetLocalPosition(0, 50 + (i * 66), 0);
		childObj->SetSortOrder(2);
		image->SetColor(1, 1, 1, 0);

		textObj = GameObject::Instantiate("Option Text");
		textObj->GetTransform()->SetParent(childObj->GetTransform());
		text = Text::Create();
		text->SetString(L"옵션");
		text->SetAligement(Text::Aligement::LEFT_MIDDLE);
		text->SetColor(1.f, 1.f, 1.0f, 1);
		text->SetSize(25);
		textObj->AddComponent(text);
		text->SetInteractable(false);
		textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		textObj->SetSortOrder(3);

		btn = Button::Create();
		childObj->AddComponent(btn);

		btn->onMouseEnter += [&]() {
			float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
			_btnOverAudio->SetVolume(effectVolume);
			_btnOverAudio->Play();
			
			GameObject::FindObjectByName("Option Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 1);
			GameObject* textObj = GameObject::FindObjectByName("Option Text");
			textObj->GetComponent<UI::Text>()->SetColor(0.4f, 0.1f, 0.1f, 1);
			textObj->GetTransform()->SetLocalPosition(80, 0, 0);
		};

		btn->onMouseLeave += [&]() {
			GameObject::FindObjectByName("Option Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("Option Text");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		};
		btn->onMouseUp += [&]() {
			_btnAudio->Play();
			OptionPopup::This()->Show();
		};
		i++;

		childObj = GameObject::Instantiate("QuitGame Btns");
		childObj->GetTransform()->SetParent(obj->GetTransform());
		image = UI::Image::Create();
		image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\MenuRowBox.dds"));
		childObj->AddComponent(image);
		childObj->GetComponent<RectTransform>()->SetPivot(0, 0.5f);
		childObj->GetComponent<RectTransform>()->SetHeight(64);
		childObj->GetComponent<RectTransform>()->SetWidth(WINCX * 0.25f);
		childObj->GetTransform()->SetLocalPosition(0, 50 + (i * 66), 0);
		childObj->SetSortOrder(2);
		image->SetColor(1, 1, 1, 0);

		textObj = GameObject::Instantiate("QuitGameText");
		textObj->GetTransform()->SetParent(childObj->GetTransform());
		text = Text::Create();
		text->SetString(L"나가기");
		text->SetAligement(Text::Aligement::LEFT_MIDDLE);
		text->SetColor(1.f, 1.f, 1.0f, 1);
		text->SetSize(25);
		textObj->AddComponent(text);
		text->SetInteractable(false);
		textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		textObj->SetSortOrder(3);


		btn = Button::Create();
		childObj->AddComponent(btn);

		btn->onMouseEnter += [&]() {
			float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
			_btnOverAudio->SetVolume(effectVolume);
			_btnOverAudio->Play();
			
			GameObject::FindObjectByName("QuitGame Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 1);
			GameObject* textObj = GameObject::FindObjectByName("QuitGameText");
			textObj->GetComponent<UI::Text>()->SetColor(0.4f, 0.1f, 0.1f, 1);
			textObj->GetTransform()->SetLocalPosition(80, 0, 0);
		};

		btn->onMouseLeave += [&]() {
			GameObject::FindObjectByName("QuitGame Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("QuitGameText");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		};

		btn->onMouseUp += [&]() {
			GameObject::FindObjectByName("QuitGame Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("QuitGameText");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
			Application::This().Close();
		};
			
		i++;

		childObj = GameObject::Instantiate("Credits Btns");
		childObj->GetTransform()->SetParent(obj->GetTransform());
		image = UI::Image::Create();
		image->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\TitleScene\\MenuRowBox.dds"));
		childObj->AddComponent(image);
		childObj->GetComponent<RectTransform>()->SetPivot(0, 0.5f);
		childObj->GetComponent<RectTransform>()->SetHeight(64);
		childObj->GetComponent<RectTransform>()->SetWidth(WINCX * 0.25f);
		childObj->GetTransform()->SetLocalPosition(0, 50 + (i * 66), 0);
		childObj->SetSortOrder(2);
		image->SetColor(1, 1, 1, 0);

		textObj = GameObject::Instantiate("Credits Text");
		textObj->GetTransform()->SetParent(childObj->GetTransform());
		text = Text::Create();
		text->SetString(L"크레딧");
		text->SetAligement(Text::Aligement::LEFT_MIDDLE);
		text->SetColor(1.f, 1.f, 1.0f, 1);
		text->SetSize(25);
		textObj->AddComponent(text);
		text->SetInteractable(false);
		textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		textObj->SetSortOrder(3);

		btn = Button::Create();
		childObj->AddComponent(btn);

		btn->onMouseEnter += [&]() {
			float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
			_btnOverAudio->SetVolume(effectVolume);
			_btnOverAudio->Play();
		
			GameObject::FindObjectByName("Credits Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 1);
			GameObject* textObj = GameObject::FindObjectByName("Credits Text");
			textObj->GetComponent<UI::Text>()->SetColor(0.4f, 0.1f, 0.1f, 1);
			textObj->GetTransform()->SetLocalPosition(80, 0, 0);
		};

		btn->onMouseLeave += [&]() {
			GameObject::FindObjectByName("Credits Btns")->GetComponent<UI::Image>()->SetColor(1, 1, 1, 0);
			GameObject* textObj = GameObject::FindObjectByName("Credits Text");
			textObj->GetComponent<UI::Text>()->SetColor(1, 1, 1, 1);
			textObj->GetTransform()->SetLocalPosition(50, 0, 0);
		}; 

		btn->onMouseDown += std::bind(&TitleScene::OnCredit, this);

		i++;

	}

	return S_OK;
}

void TitleScene::FixedUpdate(float fElapsedTime) noexcept
{
	//if(INPUT->GetKeyDown(VK_RETURN))
	//	PADINPUT->Vibration(1, 1, 0.4f,CETween::EaseType::easeInBack);
}

void TitleScene::Update(float fElapsedTime) noexcept
{ 
	_Time += fElapsedTime;

	if (_Time <= 1)
	{
		float fY = CETween::Lerp(-200, WINCY * 0.25f, _Time, CETween::EaseType::easeOutBounce);
		_LogoTransform->SetWorldPosition(WINCX * 0.25f, fY, 0);
	}
}

void TitleScene::LateUpdate(float fElapsedTime) noexcept
{

}

void TitleScene::Render(void) noexcept
{

}

void TitleScene::LateRender(void) noexcept
{

}

void TitleScene::Release(void) noexcept
{

}

void TitleScene::OnClickPlayLocal(void) noexcept
{
	Application::This().SetMousePointShowState(false);

	float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
	_btnAudio->SetVolume(effectVolume);
	_btnAudio->Play();
	FadeController::This()->StartFadeOut( 1.0f
										, true
										, false
										, [&]() {
											//LoadingScene::SetNextSceneName("Fight");
											//LoadingScene::SetNextSceneName("Tree");
											LoadingScene::SetNextSceneName(PlayerPrefs::This().GetString("LastScene", "WareHouse_01").c_str());
											//LoadingScene::SetNextSceneName("Dungeon");
											//LoadingScene::SetNextSceneName("Ogre");
											//LoadingScene::SetNextSceneName("WareHouse_03");
											//LoadingScene::SetNextSceneName("WareHouse_02");
											SCENEMANAGER->LoadScene("Loading");
										});
}

void TitleScene::OnClickPlayOnline(void) noexcept
{
	Application::This().SetMousePointShowState(false);

	PlayerPrefs::This().DeleteKey("LastScene");

	float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
	_btnAudio->SetVolume(effectVolume);
	_btnAudio->Play();
	FadeController::This()->StartFadeOut( 1.0f
										, true
										, false
										, [&]() {
											//LoadingScene::SetNextSceneName("EffectTest");
											//LoadingScene::SetNextSceneName("WareHouse_01"); //변경하지마라
											//LoadingScene::SetNextSceneName("Fight");
											LoadingScene::SetNextSceneName("WareHouse_01");
											//LoadingScene::SetNextSceneName("Dungeon");
											//LoadingScene::SetNextSceneName("Tree");
											//LoadingScene::SetNextSceneName("WackAMole");
											//LoadingScene::SetNextSceneName("Chess");
											//LoadingScene::SetNextSceneName("Ogre");
											//LoadingScene::SetNextSceneName("Ending");
											//LoadingScene::SetNextSceneName("Final");
											SCENEMANAGER->LoadScene("Loading");
										});
}

void TitleScene::OnClickOption(void) noexcept
{

}

void TitleScene::OnCredit(void) noexcept
{
	Application::This().SetMousePointShowState(false);

	float effectVolume = 2 * PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
	_btnAudio->SetVolume(effectVolume);
	_btnAudio->Play();
	FadeController::This()->StartFadeOut(1.0f
		, true
		, false
		, [&]() {
			LoadingScene::SetNextSceneName("Credit");
			SCENEMANAGER->LoadScene("Loading");
		});
}
