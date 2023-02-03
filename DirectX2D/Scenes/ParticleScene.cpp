#include "Framework.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene()
{    
    quad = new Quad(L"Textures/Effect/Boxpiece1.png");
    //quad->SetVertexShader(L"Particle.hlsl");
    //quad->SetPixelShader(L"Particle.hlsl");
    quad->SetVertexShader(L"Instancing.hlsl");
    quad->SetPixelShader(L"Instancing.hlsl");
    //quad->SetTexture(L"Textures/Effect/Boxpiece1.png");

    for (int i = 1; i < 7; i++)
    {
        wstring file = L"Textures/Effect/Boxpiece" + to_wstring(i) + L".png";
        textures.push_back(Texture::Add(file));
        textureArray.push_back(Texture::Add(file)->GetSRV());
    }

    Start();

    instanceBuffer = new VertexBuffer(instances.data(),
        sizeof(InstanceData), MAX_PARTICLE);

    char path[128];
    GetCurrentDirectoryA(sizeof(path), path);
    projectPath = path;   
    
}

ParticleScene::~ParticleScene()
{
    delete quad;
    delete instanceBuffer;
}

void ParticleScene::Update()
{
    Play();
}

void ParticleScene::Render()
{
    DC->PSSetShaderResources(1, textureArray.size(), textureArray.data());

    instanceBuffer->Set(1);

    quad->SetRender();

    DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);
}

void ParticleScene::PostRender()
{
    ImGui::Text("Particle Editor");

    EditTexture();

    ImGui::SliderInt("ParticleCount", (int*)&particleCount, 1, MAX_PARTICLE);
    ImGui::DragFloat("Duration", &data.duration, 0.1f, 10.0f);
    ImGui::ColorEdit4("StartColor", (float*)&data.startColor);
    ImGui::ColorEdit4("EndColor", (float*)&data.endColor);
    ImGui::SliderFloat2("MinVelocity", (float*)&data.minVelocity, -1, 1);
    ImGui::SliderFloat2("MaxVelocity", (float*)&data.maxVelocity, -1, 1);
    ImGui::SliderFloat2("MinAccelation", (float*)&data.minAccelation, -5, 5);
    ImGui::SliderFloat2("MaxAccelation", (float*)&data.maxAccelation, -5, 5);
    ImGui::SliderFloat("MinAngularVelocity", (float*)&data.minAngularVelocity, -10, 10);
    ImGui::SliderFloat("MaxAngularVelocity", (float*)&data.maxAngularVelocity, -10, 10);
    ImGui::SliderFloat("MinSpeed", (float*)&data.minSpeed, 0, data.maxSpeed);
    ImGui::SliderFloat("MaxSpeed", (float*)&data.maxSpeed, data.minSpeed, 200);
    ImGui::SliderFloat("MinStartTime", (float*)&data.minStartTime, 0.0f, data.maxStartTime);
    ImGui::SliderFloat("MaxStartTime", (float*)&data.maxStartTime, data.minStartTime, data.duration);
    ImGui::SliderFloat2("MinScale", (float*)&data.minScale, 0, 5);
    ImGui::SliderFloat2("MaxScale", (float*)&data.maxScale, 0, 5);

    SaveDialog();
    ImGui::SameLine();
    LoadDialog();
}

void ParticleScene::Play()
{
    lifeTime += DELTA;

    color.x = Lerp(data.startColor.x, data.endColor.x, lifeTime / data.duration);
    color.y = Lerp(data.startColor.y, data.endColor.y, lifeTime / data.duration);
    color.z = Lerp(data.startColor.z, data.endColor.z, lifeTime / data.duration);
    color.w = Lerp(data.startColor.w, data.endColor.w, lifeTime / data.duration);

    for (UINT i = 0; i < data.count; i++)
    {
        if (particleInfos[i].startTime > lifeTime)
        {
            transforms[i].Scale() = { 0, 0 };
            transforms[i].UpdateWorld();
            instances[i].transform = XMMatrixTranspose(transforms[i].GetWorld());
            continue;
        }

        particleInfos[i].velocity += particleInfos[i].accelation * DELTA;
        transforms[i].Pos() += particleInfos[i].velocity * particleInfos[i].speed * DELTA;
        transforms[i].Rot().z += particleInfos[i].angularVelocity * DELTA;
        transforms[i].Scale() = particleInfos[i].scale;
        //transforms[i].Scale() = textures[particleInfos[i].textureIndex]->GetSize();
        transforms[i].UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transforms[i].GetWorld());

        instances[i].textureIndex = particleInfos[i].textureIndex;
    }

    quad->GetColor() = color;
    instanceBuffer->Update(instances.data(), instances.size());

    if (lifeTime > data.duration)
        Start();
}

void ParticleScene::UpdateParticleInfo()
{
    instances.resize(data.count);
    particleInfos.resize(data.count);
    transforms.resize(data.count);

    color = data.startColor;

    for (ParticleInfo& info : particleInfos)
    {
        info.velocity = Random(data.minVelocity, data.maxVelocity);
        info.accelation = Random(data.minAccelation, data.maxAccelation);
        info.angularVelocity = Random(data.minAngularVelocity, data.maxAngularVelocity);
        info.speed = Random(data.minSpeed, data.maxSpeed);
        info.startTime = Random(data.minStartTime, data.maxStartTime);
        info.scale = Random(data.minScale, data.maxScale);
        info.textureIndex = Random(0, textureArray.size());

        info.velocity.Normalize();
    }
}

void ParticleScene::Start()
{
    lifeTime = 0.0f;
    data.count = particleCount;

    for (Transform& transform : transforms)
    {
        transform.Pos() = startPos;
        transform.Rot() = { 0, 0, 0 };
        transform.Scale() = { 1, 1 };
    }

    UpdateParticleInfo();
}

void ParticleScene::EditTexture()
{
    ImTextureID textureID = quad->GetTexture()->GetSRV();

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
    {
        DIALOG->OpenDialog("SelectTexture", "SelectTexture", ".png, .jpg", ".");
    }

    if (DIALOG->Display("SelectTexture"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            quad->SetTexture(Texture::Add(ToWString(file)));
        }

        DIALOG->Close();
    }
}

void ParticleScene::SaveData(string file)
{
    BinaryWriter* writer = new BinaryWriter(file);

    writer->WString(quad->GetTexture()->GetFile());

    writer->Byte(&data, sizeof(ParticleData));

    delete writer;
}

void ParticleScene::LoadData(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    wstring textureFile = reader->WString();
    quad->SetTexture(Texture::Add(textureFile));

    ParticleData* particleData = new ParticleData();
    reader->Byte((void**)&particleData, sizeof(ParticleData));

    data = *particleData;

    UINT temp = data.count;
    data.count = particleCount;
    particleCount = temp;

    delete reader;
}

void ParticleScene::SaveDialog()
{
    if (ImGui::Button("Save"))
    {
        DIALOG->OpenDialog("Save", "Save", ".fx", ".");
    }

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            SaveData(file);
        }

        DIALOG->Close();
    }
}

void ParticleScene::LoadDialog()
{
    if (ImGui::Button("Load"))
    {
        DIALOG->OpenDialog("Load", "Load", ".fx", ".");
    }

    if (DIALOG->Display("Load"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            LoadData(file);
        }

        DIALOG->Close();
    }
}
