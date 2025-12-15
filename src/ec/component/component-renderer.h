#pragma once
#include "ec/component/component.h"
#include <glad/glad.h>
#include "ec/component/material/component-material.h"
#include "rect.h"
#include <variant>
#include <algorithm>

constexpr auto VERTICES = 4u;  // Only 4 vertices per quad when using an index buffer

namespace Component {
    // Render command structure for deferred batching
    struct RenderCommand {
        int layer;
        float sort_y;  // Y position for sorting (lower Y = drawn first/behind)
        glm::vec4 src;
        std::variant<glm::vec4, std::array<glm::vec2, 4>> dest;  // Support both dest formats
        IMaterial* material;
        
        // Comparison for sorting: first by layer, then by sort_y
        bool operator<(const RenderCommand& other) const {
            if (layer != other.layer) {
                return layer < other.layer;
            }
            return sort_y < other.sort_y;
        }
    };
    class Renderer : public IComponent {
        GLuint vbo_, vao_, ebo_, att_size_, max_sprites_;
        std::vector<GLfloat> buffer_;
        std::vector<GLuint> indices_;
        IMaterial* current_mat_;
        std::vector<RenderCommand> render_commands_;  // Deferred render commands
        
        // Helper method to submit a command's vertices to the buffer
        void submit_command_to_buffer(const RenderCommand& cmd) {
            // Check if buffer is over sprite limit or current material isn't set or is different
            if ((this->buffer_.size() >= static_cast<std::size_t>(this->max_sprites_) * VERTICES * att_size_) || !this->current_mat_ || this->current_mat_->id != cmd.material->id) {
                this->flush();  // Flush out current batch and start on the next one
                this->current_mat_ = cmd.material;
            }
            
            // Handle both dest formats
            if (std::holds_alternative<glm::vec4>(cmd.dest)) {
                glm::vec4 dest = std::get<glm::vec4>(cmd.dest);
                // Bottom-left
                buffer_.push_back(dest.x);
                buffer_.push_back(dest.w);
                buffer_.push_back(cmd.src.x);
                buffer_.push_back(cmd.src.w);

                // Bottom-right
                buffer_.push_back(dest.z);
                buffer_.push_back(dest.w);
                buffer_.push_back(cmd.src.z);
                buffer_.push_back(cmd.src.w);

                // Top-left
                buffer_.push_back(dest.x);
                buffer_.push_back(dest.y);
                buffer_.push_back(cmd.src.x);
                buffer_.push_back(cmd.src.y);

                // Top-right
                buffer_.push_back(dest.z);
                buffer_.push_back(dest.y);
                buffer_.push_back(cmd.src.z);
                buffer_.push_back(cmd.src.y);
            } else {
                std::array<glm::vec2, 4> dest = std::get<std::array<glm::vec2, 4>>(cmd.dest);
                // Bottom-left
                buffer_.push_back(dest[0].x);
                buffer_.push_back(dest[0].y);
                buffer_.push_back(cmd.src.x);
                buffer_.push_back(cmd.src.w);

                // Bottom-right
                buffer_.push_back(dest[1].x);
                buffer_.push_back(dest[1].y);
                buffer_.push_back(cmd.src.z);
                buffer_.push_back(cmd.src.w);

                // Top-left
                buffer_.push_back(dest[2].x);
                buffer_.push_back(dest[2].y);
                buffer_.push_back(cmd.src.x);
                buffer_.push_back(cmd.src.y);

                // Top-right
                buffer_.push_back(dest[3].x);
                buffer_.push_back(dest[3].y);
                buffer_.push_back(cmd.src.z);
                buffer_.push_back(cmd.src.y);
            }
        }
        
        // Sort commands by layer and Y position, then batch and render
        void sort_and_batch() {
            if (render_commands_.empty()) return;
            
            // Sort commands: first by layer (ascending), then by sort_y (ascending)
            std::stable_sort(render_commands_.begin(), render_commands_.end());
            
            // Process sorted commands, batching by material
            for (const auto& cmd : render_commands_) {
                submit_command_to_buffer(cmd);
            }
        }
        
    public:
        void init(nlohmann::json json, Entity* game) override {
            std::vector<unsigned> attributes = json["attributes"];
            max_sprites_ = json["max_sprites"];
            current_mat_ = nullptr;

            // Calculate total attribute size
            att_size_ = 0;
            for (auto att : attributes)
                att_size_ += att;

            // Preallocate buffer based on maximum sprites to avoid reallocations
            buffer_.reserve(max_sprites_ * VERTICES * att_size_);

            // Generate index buffer for quads (6 indices per quad, two triangles)
            for (unsigned int i = 0; i < max_sprites_; ++i) {
                unsigned int offset = i * 4;  // 4 vertices per quad

                /*indices_.push_back(offset + 0); // Bottom-left
                indices_.push_back(offset + 3); // Top-right
                indices_.push_back(offset + 2); // Top-left
                indices_.push_back(offset + 0); // Bottom-left
                indices_.push_back(offset + 1); // Bottom-right
                indices_.push_back(offset + 3); // Top-right*/

                indices_.push_back(offset + 0); // Bottom-left
                indices_.push_back(offset + 1); // Bottom-right
                indices_.push_back(offset + 2); // Top-left
                indices_.push_back(offset + 1); // Bottom-right
                indices_.push_back(offset + 3); // Top-right
                indices_.push_back(offset + 2); // Top-left
            }

            // Generate buffers
            glGenVertexArrays(1, &vao_);
            glGenBuffers(1, &vbo_);
            glGenBuffers(1, &ebo_);

            // Bind buffers
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

            // Allocate immutable storage for the vertex buffer
            GLbitfield storageFlags = GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT;
            glBufferStorage(GL_ARRAY_BUFFER, max_sprites_ * VERTICES * att_size_ * sizeof(GLfloat), nullptr, storageFlags);

            // Create and bind attributes
            size_t offset = 0;
            for (size_t i = 0; i < attributes.size(); ++i) {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, att_size_ * sizeof(float), (GLvoid*)(offset));
                offset += attributes[i] * sizeof(float);
            }

            // Upload the element (index) buffer data
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);
        }

        Renderer() : vbo_(0), vao_(0), ebo_(0), current_mat_(nullptr), max_sprites_(0), att_size_(0) {}
        ~Renderer() { release(); }

        void release() {
            // Delete buffers if they exist
            if (vbo_) glDeleteBuffers(1, &vbo_);
            if (vao_) glDeleteVertexArrays(1, &vao_);
            if (ebo_) glDeleteBuffers(1, &ebo_);
        }

        void begin() {
            // Clear render commands for a new frame
            render_commands_.clear();
            // Pre-allocate to avoid reallocations (estimate based on max_sprites_)
            render_commands_.reserve(max_sprites_);
            // Ensure the current material is cleared for a new batch
            current_mat_ = nullptr;
        }

        void draw(glm::vec4 src, std::array<glm::vec2, 4> dest, IMaterial* mat, int layer = 0)
        {
            // Calculate sort_y from the highest Y coordinate (top of sprite)
            float sort_y = std::max(std::max(dest[0].y, dest[1].y), std::max(dest[2].y, dest[3].y));
            
            // Create and store render command for deferred batching
            RenderCommand cmd;
            cmd.layer = layer;
            cmd.sort_y = sort_y;
            cmd.src = src;
            cmd.dest = dest;
            cmd.material = mat;
            
            render_commands_.push_back(cmd);
        }

        void draw(glm::vec4 src, glm::vec4 dest, IMaterial* mat, int layer = 0) {
            // Calculate sort_y from top Y coordinate (dest.y is top)
            float sort_y = dest.w;
            
            // Create and store render command for deferred batching
            RenderCommand cmd;
            cmd.layer = layer;
            cmd.sort_y = sort_y;
            cmd.src = src;
            cmd.dest = dest;
            cmd.material = mat;
            
            render_commands_.push_back(cmd);
        }

        void flush() {
            if (this->buffer_.empty()) return;

            // Make sure we have a material to use
            if (!this->current_mat_) {
                this->buffer_.clear();
                return;
            }

            // Set uniforms and use shader
            this->current_mat_->compile();
            this->current_mat_->bind();

            // Bind VBO and update buffer data
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            if (ptr) {
                std::memcpy(ptr, buffer_.data(), buffer_.size() * sizeof(GLfloat));
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
            else
                Logger::error("Failed to map buffer!", Logger::HIGH);


            size_t numIndices = (buffer_.size() / att_size_) * 6 / VERTICES;
            // Draw indexed quads using glDrawElements
            glDrawElements(GL_TRIANGLES, (GLsizei)(numIndices), GL_UNSIGNED_INT, nullptr);

            //glDrawElements(GL_TRIANGLES, (GLsizei)(indices_.size()), GL_UNSIGNED_INT, nullptr);


            // Clear buffer for the next batch
            this->buffer_.clear();
        }

        void end() {
            // Sort commands and batch them
            sort_and_batch();
            // Flush any remaining sprites to be drawn
            this->flush();
        }

        std::string get_id() override { return "renderer"; }
    };
}