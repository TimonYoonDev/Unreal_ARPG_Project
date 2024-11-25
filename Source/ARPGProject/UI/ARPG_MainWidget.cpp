#include "ARPG_MainWidget.h"

void UARPG_MainWidget::SetBind(const AARPG_Character* Character)
{
    AttributeComponent = Character->GetAttributeComponent();
    AttributeComponent->OnHealthChanged.AddUObject(this, &UARPG_MainWidget::UpdateHealthBar);
    AttributeComponent->OnStaminaChanged.AddUObject(this, &UARPG_MainWidget::UpdateStaminaBar);
    UpdateHealthBar();
    UpdateStaminaBar();
}

void UARPG_MainWidget::UpdateHealthBar() const
{
    HealthBar->SetPercent(AttributeComponent->GetHealthPercent());
}

void UARPG_MainWidget::UpdateStaminaBar() const
{
    StaminaBar->SetPercent(AttributeComponent->GetStaminaPercent());
}
