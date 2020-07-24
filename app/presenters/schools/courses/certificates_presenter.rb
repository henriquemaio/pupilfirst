module Schools
  module Courses
    class CertificatesPresenter < ApplicationPresenter
      def initialize(view_context, course)
        @course = course

        super(view_context)
      end

      def props
        {
          course: course_details,
          certificates: certificates,
        }
      end

      def certificates
        active_certificate = @course.certificates.active.limit(1)
        ActiveRecord::Precounter.new(active_certificate).precount(:issued_certificates)

        inactive_certificates = @course.certificates.inactive.order(updated_at: :desc).limit(9)
        ActiveRecord::Precounter.new(inactive_certificates).precount(:issued_certificates)

        latest_certificates = active_certificate.to_a + inactive_certificates.to_a
        latest_certificates.map { |c| certificate_details(c) }
      end

      private

      def certificate_details(certificate)
        certificate.attributes.slice(
          'id', 'name', 'qr_corner', 'qr_scale', 'name_offset_top', 'font_size', 'margin', 'active', 'created_at', 'updated_at'
        ).merge(
          image_url: certificate.image_path,
          issued_certificates_count: certificate.issued_certificates_count
        )
      end

      def course_details
        {
          id: @course.id,
          name: @course.name
        }
      end
    end
  end
end
