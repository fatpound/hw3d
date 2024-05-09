#include "Camera.hpp"

#include "../../../../imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto& pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r_, 0.0f), // setting the camera at a distance of r_
		dx::XMMatrixRotationRollPitchYaw(phi_, -theta_, 0.0f)
	);

	const auto& look =
		dx::XMMatrixLookAtLH(
			pos,                // position to go n look from
			dx::XMVectorZero(), // position to go n look at
			dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // y should be pointing towards up
		) *
		dx::XMMatrixRotationRollPitchYaw(pitch_, -yaw_, roll_);

	return look;
}

void Camera::SpawnControlImguiWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r_, 0.1f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta_, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi_, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll_, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch_, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw_, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}
void Camera::Reset() noexcept
{
	r_     = 20.0f;
	theta_ =  0.0f;
	phi_   =  0.0f;
	pitch_ =  0.0f;
	yaw_   =  0.0f;
	roll_  =  0.0f;
}
