
#include "SVerticalBoxEx.h"

#define  LOCTEXT_NAMESPACE "SVerticalBoxEx"

void SVerticalBoxEx::Construct(const FArguments& InArgs)
{
	//const FVector2D EditBoxBg(642, 152);
	//FSlateImageBrush(Style->RootToContentDir(TEXT("bg1"), TEXT(".png")), EditBoxBg);

	m_ContentMargin = InArgs._ContentMargin.Get();
	m_BGSize = InArgs._BGSize.Get();

	//尝试最后添加一个背景图放在最下面
	SVerticalBoxEx::FSlot& NewSlot = *new SVerticalBoxEx::FSlot();
	NewSlot
	[
		SNew(SImage).Image(InArgs._BGImage.Get())
	];
	Children.Add(&NewSlot);
	

	const int32 NumSlots = InArgs.Slots.Num();
	for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
	{
		Children.Add(InArgs.Slots[SlotIndex]);
	}
	
}

void SVerticalBoxEx::OnArrangeChildren(const FGeometry & AllottedGeometry, FArrangedChildren & ArrangedChildren) const
{
	FVector2D BoxSize = AllottedGeometry.GetLocalSize();
	float startX = m_ContentMargin.Left;
	float startY = m_ContentMargin.Top;
	float currentMaxHeight = 0.0f;

	//先把背景加入
	if (Children.Num() > 0)
	{
		const SBoxPanel::FSlot& BgSlot = Children[0];
		const EVisibility BgVisibility = BgSlot.GetWidget()->GetVisibility();

		//获取背景的尺寸
		FVector2D size = BgSlot.GetWidget()->GetDesiredSize();

		//如果没有传，则默认使用原始的
		if (m_BGSize.IsZero() == false)
		{
			size = m_BGSize;
		}	

		if (ArrangedChildren.Accepts(BgVisibility))
			ArrangedChildren.AddWidget(BgVisibility, AllottedGeometry.MakeChild(
				BgSlot.GetWidget(), FVector2D(0, 0), FVector2D(size)
			));
	}

	//对每一个子元素排序，不包括一开始加入的背景
	for (int count = 1; count < Children.Num() ; count ++)
	{
		const SBoxPanel::FSlot &curSlot = Children[count];
		const EVisibility SlotVisibility = curSlot.GetWidget()->GetVisibility();

		FVector2D size = curSlot.GetWidget()->GetDesiredSize();

		if (ArrangedChildren.Accepts(SlotVisibility))
		{
			ArrangedChildren.AddWidget(SlotVisibility, AllottedGeometry.MakeChild(
				curSlot.GetWidget(), FVector2D(startX, startY), FVector2D(size)
			));

			startY += (size.Y + m_ContentMargin.Bottom);
			//startX += (size.X + m_Margin.Left);
		}
	}


}

FReply SVerticalBoxEx::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	return Reply;
}



#undef   LOCTEXT_NAMESPACE