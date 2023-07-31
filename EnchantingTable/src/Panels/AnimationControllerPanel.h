#pragma once

#include <Arcane.h>
#include <string>
#include <ImGradient.h>
#include <GraphEditor.h>

template <typename T, std::size_t N>
struct Array
{
    T data[N];
    const size_t size() const { return N; }

    const T operator [] (size_t index) const { return data[index]; }
    operator T* () {
        T* p = new T[N];
        memcpy(p, data, sizeof(data));
        return p;
    }
};

template <typename T, typename ... U> Array(T, U...) -> Array<T, 1 + sizeof...(U)>;
struct GraphEditorDelegate : public GraphEditor::Delegate
{
    bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
    {
        return true;
    }

    void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
    {
        mNodes[nodeIndex].mSelected = selected;
    }

    void MoveSelectedNodes(const ImVec2 delta) override
    {
        for (auto& node : mNodes)
        {
            if (!node.mSelected)
            {
                continue;
            }
            node.x += delta.x;
            node.y += delta.y;
        }
    }

    virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
    {
    }

    void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
    {
        mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });

        // This is if we are connecting to the entry node
        if (inputNodeIndex == 0) {
            Node currentNode = mNodes[outputNodeIndex];
            controller->SetCurrentAnimation(currentNode.animation->GetName());
        }
        else {
            Node fromNode = mNodes[inputNodeIndex];
            Node toNode = mNodes[outputNodeIndex];

            Arcane::AnimationLink newLink;
            newLink.from = fromNode.animation;
            newLink.to = toNode.animation;

            controller->AddLink(newLink);
        }
    }

    void DelLink(GraphEditor::LinkIndex linkIndex) override
    {
        mLinks.erase(mLinks.begin() + linkIndex);
    }

    void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override
    {
        drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
    }

    const size_t GetTemplateCount() override
    {
        return sizeof(mTemplates) / sizeof(GraphEditor::Template);
    }

    const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
    {
        return mTemplates[index];
    }

    const size_t GetNodeCount() override
    {
        return mNodes.size();
    }

    const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
    {
        const auto& myNode = mNodes[index];
        return GraphEditor::Node
        {
            myNode.name,
            myNode.templateIndex,
            ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
            myNode.mSelected,
        };
    }

    const size_t GetLinkCount() override
    {
        return mLinks.size();
    }

    const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
    {
        return mLinks[index];
    }

    // Graph datas
    static const inline GraphEditor::Template mTemplates[] = {
        // Entry Node Template
        {
            IM_COL32(160, 160, 180, 255),
            IM_COL32(100, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            0,
            nullptr,
            nullptr,
            1,
            Array{"Output"},
            nullptr
        },
        // Other node index
        {
            IM_COL32(160, 160, 180, 255),
            IM_COL32(100, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            1,
            Array{"Input"},
            nullptr,
            1,
            Array{"Output"},
            nullptr
        },
    };

    struct Node
    {
        const char* name;
        GraphEditor::TemplateIndex templateIndex;
        float x, y;
        bool mSelected;
        Arcane::Animation* animation;
    };

    std::vector<Node> mNodes = {
        {
            "Entry",
            0,
            0, 0,
            false
        }
    };

    std::vector<GraphEditor::Link> mLinks = { };
    Arcane::AnimationController* controller = nullptr;
};



class AnimationControllerPanel
{
public:
	AnimationControllerPanel();

	void OnImGuiRender();

	void SetAnimationController(Arcane::AnimationController* controller);
private:
    void UpdateAnimationController();
private:
	Arcane::AnimationController* m_AnimationController = nullptr;
    GraphEditorDelegate m_Delegate;
    GraphEditor::FitOnScreen m_Fit;
    GraphEditor::Options m_Options;
    GraphEditor::ViewState m_ViewState;
};